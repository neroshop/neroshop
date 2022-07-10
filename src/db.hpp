#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <xapian.h>

#include "logging.hpp"

namespace neroshop {

class document {
  public:
    document( const std::string& author,
              const std::string& title,
              const std::string& description )
      : m_author( author )
      , m_title( title )
      , m_description( description )
      {}

  private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template< class Archive >
    void serialize( Archive & ar, const unsigned int version ) {
      ar & m_author;
      ar & m_title;
      ar & m_description;
    }

    std::string m_author;
    std::string m_title;
    std::string m_description;
};

Xapian::doccount get_doccount( const std::string db_name );

void index_db( const std::string& db_name, const std::string& input_filename );

std::string db_query( const std::string& db_name, std::string&& query_string );

std::string db_add( const std::string& db_name, std::string&& cmd_string );

} // neroshop::
