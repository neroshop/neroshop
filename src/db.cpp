
#include "db.hpp"

// ****************************************************************************
Xapian::doccount
neroshop::get_doccount( const std::string db_name ) {
  try {
    Xapian::Database db( db_name );
    return db.get_doccount();
  } catch ( const Xapian::Error &e ) {
    NLOG(ERROR) << e.get_description();
  }
  return {};
}

// ****************************************************************************
void
neroshop::index_db( const std::string& db_name,
                    const std::string& input_filename )
{
  if (input_filename.empty()) return;
  NLOG(DEBUG) << "Indexing " << input_filename;
  Xapian::WritableDatabase db( db_name, Xapian::DB_CREATE_OR_OPEN );
  Xapian::TermGenerator indexer;
  Xapian::Stem stemmer( "english" );
  indexer.set_stemmer( stemmer );
  indexer.set_stemming_strategy( indexer.STEM_SOME_FULL_POS );
  std::ifstream cin( input_filename );
  try {
    std::string para;
    std::size_t lcnt = 0, pcnt = 0;
    while (true) {
      std::string line;
      if (cin.eof()) {
        if (para.empty()) break;
      } else {
        getline( cin, line );
        cin.ignore();
        ++lcnt;
      }
      if (line.empty()) {
        if (!para.empty()) {
          // We've reached the end of a paragraph, so index it
          Xapian::Document doc;
          doc.set_data(para);
          indexer.set_document( doc );
          indexer.index_text( para );
          // Add the document to the database
          db.add_document( doc );
          para.resize( 0 );
          ++pcnt;
        }
      } else {
        if (!para.empty()) para += '\n';
        para += line;
      }
    }
    NLOG(INFO) << "Finished indexing " + std::to_string(lcnt) << " lines and "
               << std::to_string(pcnt) << " paragraphs, commit to db";
    // Explicitly commit so that we get to see any errors. WritableDatabase's
    // destructor will commit implicitly (unless we're in a transaction) but
    // will swallow any exceptions produced.
    db.commit();
  } catch ( const Xapian::Error &e ) {
    NLOG(ERROR) << e.get_description();
  }
}

// *****************************************************************************
std::string
neroshop::query_db( const std::string& db_name, std::string&& query_string) {
  try {
    query_string.erase( 0, 3 );
    NLOG(DEBUG) << "Search db user query '" << query_string << "'";
    // Open the database for searching
    Xapian::Database db( db_name );
    // Start an enquire session
    Xapian::Enquire enquire( db );
    // Parse the query string to produce a Xapian::Query object
    Xapian::QueryParser qp;
    Xapian::Stem stemmer("english");
    qp.set_stemmer( stemmer );
    qp.set_database( db );
    qp.set_stemming_strategy( Xapian::QueryParser::STEM_SOME );
    Xapian::Query query = qp.parse_query( query_string );
    NLOG(DEBUG) << "Parsed query: '" << query.get_description() << "'";
    // Find the top 10 results for the query
    enquire.set_query( query );
    NLOG(DEBUG) << "Set query: '" << query.get_description() << "'";
    Xapian::MSet matches = enquire.get_mset( 0, 10 );
    NLOG(DEBUG) << "Got matches";
    // Construct the results
    auto nr = matches.get_matches_estimated();
    NLOG(DEBUG) << "Got estimated matches: " << nr;
    std::stringstream result;
    result << nr << " results found.";
    if (nr) {
      result << "\nMatches 1-" << matches.size() << ":\n\n";
      for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
        NLOG(DEBUG) << "Getting match: " << i.get_rank();
        result << i.get_rank() + 1 << ": " << i.get_weight() << " docid=" << *i
                 << " [" << i.get_document().get_data() << "]\n";
      }
    }
    NLOG(DEBUG) << "Results: " + result.str();
    return result.str();
  } catch ( const Xapian::Error &e ) {
    NLOG(ERROR) << e.get_description();
  }
  return {};
}
