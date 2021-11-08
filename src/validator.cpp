#include "../include/validator.hpp"

////////////////////
////////////////////
////////////////////
////////////////////
bool Validator::register_user(const std::string& username, const std::string& password, const std::string& confirm_pw, std::string opt_email) // user would have to confirm their pw twice to make sure they match :O - I totally forgot!
{
    // username (will appear only in lower-case letters within the app)
    if(!validate_username(username)) {return false;}
    // password
    if(!validate_password(password)) {return false;}
    if(password != confirm_pw){NEROSHOP_TAG std::cout << "\033[0;31;49m" << "Both passwords do not match. Try again" << "\033[0m" << std::endl;return false;}
    // email
    if(!opt_email.empty()) {
        if(!validate_email(opt_email)) {return false;}
        neroshop::print("email opted", 3);
    }
    // generate bcrypt salt (random)
	char salt[BCRYPT_HASHSIZE];//char * salt;
    if(!generate_bcrypt_salt(12, salt)) {return false;}
    // generate bcrypt hash (from password and salt combination)
    char pw_hash[BCRYPT_HASHSIZE];//char * pw_hash;
    if(!generate_bcrypt_hash(password.c_str(), salt, pw_hash)) {return false;}
    // generate sha256 hash (from email)(this is to hide the email from prying eyes)
    std::string email_hash;
    if(!generate_sha256_hash_evp(opt_email, email_hash)) {return false;}
    // saving (save user information to database)
    save_user(username, pw_hash, email_hash);//{return;}
#ifdef NEROSHOP_DEBUG
    NEROSHOP_TAG std::cout << get_date() << "\033[1;32;49m" << " account registered" << "\033[0m" << std::endl;
    NEROSHOP_TAG std::cout << "\033[1;34;49m" << ((!username.empty()) ? std::string("Welcome to neroshop, " + username) : "Welcome to neroshop") << "\033[0m" << std::endl;
#endif
    return true;
} //ELI5: https://auth0.com/blog/adding-salt-to-hashing-a-better-way-to-store-passwords/  why bcrypt: https://security.stackexchange.com/questions/4781/do-any-security-experts-recommend-bcrypt-for-password-storage/6415#6415
////////////////////
void Validator::save_user(const std::string& username, const char pw_hash[BCRYPT_HASHSIZE], std::string email_hash) 
{
    // sqlite - save a copy of user's account details locally (on user's device)
    DB db("neroshop.db");
    // find a way to check whether database is locked so we know if user is registered or not // Register A Callback To Handle SQLITE_BUSY Errors//sqlite3_busy_handler(db.get_handle(), /*sqlite3*,int(*)(void*,int),void**/);
    //db.execute("PRAGMA journal_mode = WAL;"); // this may reduce the incidence of SQLITE_BUSY errors (such as database being locked) //https://www.sqlite.org/pragma.html#pragma_journal_mode //https://github.com/mapbox/node-sqlite3/issues/9 //sqlite3_busy_timeout(db.get_handle(), 0/*sqlite3*, int ms*/);// arg of 0 or negative turns off all busy handlers//returns int
    // create table Role if it does not yet exist
    if(!db.table_exists("role")) {
        db.table("role");
        db.column("role", "ADD", "role_name", "TEXT");
        // these rows are only inserted once since roles are static and do not change // 2 roles (1=buyer, 2=seller)
        db.insert("role", "role_name", DB::to_sql_string("Buyer"));
        db.insert("role", "role_name", DB::to_sql_string("Seller"));
    }    
    // create table of Users
    if(!db.table_exists("users")) {
        db.table("users");
        db.column("users", "ADD", "name", "TEXT");
        db.column("users", "ADD", "pw_hash", "TEXT");
        db.column("users", "ADD", "email_hash", "TEXT");
        db.column("users", "ADD", "role_id", "INTEGER");//db.column("users", "ADD", "role", "TEXT");//db.column("users", "ADD", "created_at", "TEXT");
        db.execute("CREATE UNIQUE INDEX idx_users_name ON Users (name);");// enforce that the user names are unique, in case there is an attempt to insert a new "name" of the same value - https://www.sqlitetutorial.net/sqlite-index/
    }
    std::string blank = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"; // empty string
    db.insert("users", "name, pw_hash, email_hash, role_id", DB::to_sql_string(String::lower(username)) + ", " + DB::to_sql_string(pw_hash) + ", " + ((email_hash == blank) ? DB::to_sql_string("") : DB::to_sql_string(email_hash)) + ", " + std::to_string(1)); // usernames are stored in lowercase
    db.close(); // don't forget to close the db after you're done :)
}
////////////////////
bool Validator::login(const std::string& username, const std::string& password)
{
	// sqlite - figure out how to retrieve the user's name and pw_hash from the db
    DB db("neroshop.db");
	if(!db.table_exists("users")) {NEROSHOP_TAG std::cout << "\033[0;33;49m" << "This account is not registered" << "\033[0m" << std::endl; return false;}
	// SELECT pw_hash from Users WHERE name=username
	std::string pw_hash = db.get_column_text("users", "pw_hash", "name = " + DB::to_sql_string(String::lower(username)));
	// if no pw_hash could be found then the user does not exist (or the db is missing or corrupted)
	if(pw_hash.empty()) { NEROSHOP_TAG std::cout << "\033[0;33;49m" << "This user does not exist" << "\033[0m" << std::endl; return false; }
	std::cout << "pw_hash: " << pw_hash << " (retrieved from db) "/* + username + ")"*/ << std::endl;
	db.close(); // don't forget to close the db after you're done :)
	// validate the pw
	if(!validate_bcrypt_hash(password, pw_hash)) { //return validate_bcrypt_hash(password, account.second);
	    return false;
	}
#ifdef NEROSHOP_DEBUG
    NEROSHOP_TAG std::cout << get_date() << "\033[1;32;49m" << " successfully logged in" << "\033[0m" << std::endl;
    NEROSHOP_TAG std::cout << "\033[1;34;49m" << ((!username.empty()) ? std::string("Welcome back, " + username) : "Welcome back") << "\033[0m" << std::endl;
#endif
	return true; // default value
}
////////////////////
bool Validator::login_with_email(const std::string& email, const std::string& password) { // works! but won't be used since most users won't opt-in to use an email
    DB db("neroshop.db");
	if(!db.table_exists("users")) {NEROSHOP_TAG std::cout << "\033[0;33;49m" << "This account is not registered" << "\033[0m" << std::endl; return false;}
	// get email_hash (from email)
	std::string email_hash;
	if(!generate_sha256_hash_evp(email, email_hash)) {
	    neroshop::print("An error occured", 1);
	    return false;
	}
	// SELECT pw_hash from Users WHERE email_hash=email_hash
	std::string pw_hash = db.get_column_text("users", "pw_hash", "email_hash = " + DB::to_sql_string(email_hash));
	// if no pw_hash could be found then the user does not exist (or the db is missing or corrupted)
	if(pw_hash.empty()) { NEROSHOP_TAG std::cout << "\033[0;33;49m" << "This user does not exist" << "\033[0m" << std::endl; return false; }
	std::cout << "pw_hash: " << pw_hash << " (retrieved from db) "/* + username + ")"*/ << std::endl;
	// also, get username from db (only for display purposes)
	std::string username = db.get_column_text("users", "name", "email_hash = " + DB::to_sql_string(email_hash));
	db.close(); // don't forget to close the db after you're done :)
	// validate the pw
	if(!validate_bcrypt_hash(password, pw_hash)) { //return validate_bcrypt_hash(password, account.second);
	    return false;
	}
#ifdef NEROSHOP_DEBUG
    NEROSHOP_TAG std::cout << get_date() << "\033[1;32;49m" << " successfully logged in" << "\033[0m" << std::endl;
    NEROSHOP_TAG std::cout << "\033[1;34;49m" << ((!username.empty()) ? std::string("Welcome back, " + username) : "Welcome back") << "\033[0m" << std::endl;
#endif
	return true; // default value
}
////////////////////
void Validator::change_pw(const std::string& old_pw, const std::string& new_pw/*, const std::string& confirm_new_pw*/) 
{}
////////////////////
std::string Validator::get_date(std::string format) // get current time and date
{
	auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), format.c_str());
	return ss.str();
}
////////////////////
bool Validator::validate_username(const std::string& username) 
{
    // username (will appear only in lower-case letters within the app)
    // make sure username is at least 2 characters short (min_user_length=2)
    unsigned int min_user_length = 2;
    if(username.length() < min_user_length) {
        NEROSHOP_TAG std::cout << "Your username must be at least " << min_user_length << " characters in length" << std::endl;
        return false; // exit function
    }
    // make sure username is at least 30 characters long (max_user_length=30)
    unsigned int max_user_length = 30; // what if a company has a long name? :o // also consider the textbox's max-length
    if(username.length() > max_user_length) {
        NEROSHOP_TAG std::cout << "Your username must be at least " << max_user_length << " characters in length" << std::endl;
        return false; // exit function
    }
    for(int i = 0; i < username.length(); i++)
    {
        // make sure username does not contain any spaces //std::cout << username[i] << " - char\n";
        if(isspace(username[i])) {
            NEROSHOP_TAG std::cout << "Your username cannot contain any spaces" << std::endl;
            return false; // exit function
        }
        // make sure username can only contain letters, numbers, and these specific symbols: a hyphen, underscore, and period (-,_,.) //https://stackoverflow.com/questions/39819830/what-are-the-allowed-character-in-snapchat-username#comment97381763_41959421
        // symbols like @,#,$,etc. are invalid
        if(!isalnum(username[i])) {
            if(username[i] != '-'){
            if(username[i] != '_'){
            if(username[i] != '.'){
                NEROSHOP_TAG std::cout << "Your username cannot contain any symbols except (-, _, .): " << username[i] << std::endl;
                return false; // exit function
            }}}
        }
    }
    // make sure username begins with a letter (username cannot start with a symbol or number)
    char first_char = username.at(username.find_first_of(username));
    if(!isalpha(first_char)) {
        NEROSHOP_TAG std::cout << "Your username must begin with a letter: " << first_char << std::endl;
        return false; // exit function
    }
    // make sure username does not end with a symbol (username can end with a number, but NOT with a symbol)
    char last_char = username.at(username.find_last_of(username));
    if(!isalnum(last_char)) { //if(last_char != '_') { // underscore allowed at end of username? :o
        NEROSHOP_TAG std::cout << "Your username must end with a letter or number: " << last_char << std::endl;
        return false; // exit function //}
    }
    // the name guest is reserved for guests only, so it cannot be used by any other user
    if(String::lower(username) == "guest") {
        neroshop::print("The name \"Guest\" is reserved for guests ONLY");
        return false;
    }
    // check db to see if username is not already taken (last thing to worry about)
    DB db("neroshop.db");
	if(db.table_exists("users")) 
	{   // make sure table Users exists first
	    std::string name = db.get_column_text("users", "name", "name='" + String::lower(username) + "'");
	    if(name == String::lower(username)) { // names are stored in lowercase
	        neroshop::print("This username is already taken", 2);
	        return false;
	    }//std::cout << "name: " << name << " (retrieved from db)" << std::endl;
	}
	// make sure any deleted user's name is not re-used
	if(db.table_exists("deleted_users")) {
	    std::string deleted_user = db.get_column_text("deleted_users", "name", "name = " + DB::to_sql_string(username));
	    if(deleted_user == String::lower(username)) {
	        neroshop::print("This username cannot be used"/*re-used"*/, 2);
            return false;
	    }
	}
	db.close(); // don't forget to close the db after you're done :)
    return true; // default return value
}
////////////////////
bool Validator::validate_password(const std::string& password) 
{
    //Minimum eight characters, at least one uppercase letter, one lowercase letter, one number and one special character:
    const std::regex pattern("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$"); // source: https://stackoverflow.com/questions/19605150/regex-for-password-must-contain-at-least-eight-characters-at-least-one-number-a
    if(!std::regex_match(password, pattern))
    {
        // figure out the specific reason why password failed to follow the regex rules  //if(password.empty()) { std::cout << "Please enter a valid password" << std::endl; return false; }
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[A-Z])"))) {
            neroshop::print("Password must have at least one upper case letter", 2);
        }
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[a-z])"))) {
            neroshop::print("Password must have at least one lower case letter", 2);
        }
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[0-9])"))) {
            neroshop::print("Password must have at least one digit", 2);
        }              
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[#?!@$%^&*-])"))) {
            neroshop::print("Password must have at least one special character", 2);
        }         
        if(password.length() < 8) {
            neroshop::print("Password must be at least 8 characters long", 2);
        } 
        //neroshop::print("Please enter a stronger password (at least 1 upper case letter, 1 lower case letter, 1 digit, and 1 special character)", 1);
        return false; // exit function
    }
    return true; // default return value
}
////////////////////
bool Validator::validate_email(const std::string& email) { // not done
    // make sure email is a valid email
    // must contain an "@" in between
    // must end with a (.com, .org, etc.)
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if(!std::regex_match(email, pattern)) {
        neroshop::print("Email address is not valid", 1);
        return false;
    }
    //neroshop::print("Email is valid", 3);
    return true;
}
////////////////////
bool Validator::validate_bcrypt_hash(const std::string& password, const std::string& hash)
{   // verify password
    int result = bcrypt_checkpw(password.c_str(), hash.c_str());
#ifdef NEROSHOP_DEBUG
	NEROSHOP_TAG std::cout << ((result != 0) ? "\033[0;31mThe password does NOT match" : "\033[0;32mThe password matches") << "\033[0m" << std::endl;
#endif
    return (result == 0);
}
////////////////////
bool Validator::generate_bcrypt_salt(unsigned int workfactor, char salt[BCRYPT_HASHSIZE])
{   // generate a salt (random)
    int result = bcrypt_gensalt(workfactor, salt); // workfactor must be between 4 and 31 - default is 12
#ifdef NEROSHOP_DEBUG
    std::cout << ((result != 0) ? "bcrypt salt failed to generate" : "bcrypt_salt: " + std::string(salt)) << std::endl; // 0=success
    // two users with the same password results in the same exact hash, which is why a unique salt must be generated
#endif
    return (result == 0);
}
////////////////////
bool Validator::generate_bcrypt_hash(const std::string& password, const char salt[BCRYPT_HASHSIZE], char hash[BCRYPT_HASHSIZE])
{   // generate hash (from password and salt combination)
    int result = bcrypt_hashpw(password.c_str(), salt, hash);
#ifdef NEROSHOP_DEBUG
    std::cout << ((result != 0) ? "bcrypt hash failed to generate" : "bcrypt_hash: " + std::string(hash)) << std::endl;
#endif
    return (result == 0);
}
////////////////////
bool Validator::validate_sha256_hash(const std::string& email, const std::string& hash) { // raw/unsalted hash
    std::string temp_hash;
    generate_sha256_hash_evp(email, temp_hash);
    return (temp_hash == hash);
}
////////////////////
bool Validator::generate_sha256_hash_legacy(const std::string& email, std::string& hashed) {
    // low-level interface (old)
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, email.c_str(), email.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    hashed = ss.str();
#ifdef NEROSHOP_DEBUG
    if(!email.empty()) std::cout << "sha256_hash: " << hashed << std::endl;
#endif
    return true;
}
////////////////////
bool Validator::generate_sha256_hash_evp(const std::string& email, std::string& hashed) {
    // EVP (recommended)
    bool success = false;
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if(context != NULL)
    {
        if(EVP_DigestInit_ex(context, EVP_sha256(), NULL))
        {
            if(EVP_DigestUpdate(context, email.c_str(), email.length()))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    std::stringstream ss;
                    for(unsigned int i = 0; i < lengthOfHash; ++i)
                    {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                    }

                    hashed = ss.str();
                    success = true;
                }
            }
        }
        EVP_MD_CTX_free(context);
    }
#ifdef NEROSHOP_DEBUG
    if(!email.empty()) std::cout << "sha256_hash: " << hashed << std::endl;
#endif    
    return success;    
}
////////////////////
////////////////////
////////////////////
////////////////////
