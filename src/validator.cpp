#include "validator.hpp"

////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::Validator::register_user(const std::string& username, const std::string& password, const std::string& confirm_pw, std::string opt_email) // user would have to confirm their pw twice to make sure they match :O - I totally forgot!
{
    // validate username (will appear only in lower-case letters within the app)
    if(!validate_username(username)) return false;
    // validate password
    if(!validate_password(password)) return false;
    if(password != confirm_pw){NEROSHOP_TAG_OUT std::cout << "\033[0;31;49m" << "Both passwords do not match. Try again" << "\033[0m" << std::endl;return false;}
    // if email is opted, validate email
    if(!opt_email.empty() && !validate_email(opt_email)) return false;
    // generate bcrypt salt (random, with a workfactor of 12 or higher)
	char salt[BCRYPT_HASHSIZE];//char * salt;
    if(!generate_bcrypt_salt(12, salt)) {return false;} // 12=1s, 14=4s, 16=16s, 18=64s
    // pre-hash password with sha256 (to circumvent bcrypt's pw length restrictions [55-72])
    std::string pw_prehash;
    if(!generate_sha256_hash(password, pw_prehash)) {return false;}
    // generate bcrypt hash (from pre-hashed password and salt combination)
    char pw_hash[BCRYPT_HASHSIZE];//char * pw_hash;
    if(!generate_bcrypt_hash(pw_prehash, salt, pw_hash)) {return false;}//if(!generate_bcrypt_hash(password, salt, pw_hash)) {return false;}
    // generate sha256 hash (from email)(this is to hide the email from prying eyes, in case of a hack)
    std::string email_hash;
    if(!generate_sha256_hash(opt_email, email_hash)) {return false;}
    // saving (save user information to database)
    save_user(username, pw_hash, (!opt_email.empty()) ? email_hash : "");
#ifdef NEROSHOP_DEBUG
	auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	std::string format = "%Y-%m-%d  %H:%M:%S %p";
	ss << std::put_time(std::localtime(&in_time_t), format.c_str());
    NEROSHOP_TAG_OUT std::cout << ss.str() << "\033[1;32;49m" << " account registered" << "\033[0m" << std::endl;
#endif    
    neroshop::print((!username.empty()) ? std::string("Welcome to neroshop, " + username) : "Welcome to neroshop", 4);
    return true;
} //ELI5: https://auth0.com/blog/adding-salt-to-hashing-a-better-way-to-store-passwords/  why bcrypt: https://security.stackexchange.com/questions/4781/do-any-security-experts-recommend-bcrypt-for-password-storage/6415#6415
////////////////////
void neroshop::Validator::save_user(const std::string& username, const char pw_hash[BCRYPT_HASHSIZE], std::string email_hash) 
{

}
////////////////////
bool neroshop::Validator::login(const std::string& username, const std::string& password)
{

    return false;	
}
////////////////////
bool neroshop::Validator::login_with_email(const std::string& email, const std::string& password) { // works! but won't be used since most users won't opt-in to use an email

    return false;	
}
////////////////////
void neroshop::Validator::change_pw(const std::string& old_pw, const std::string& new_pw/*, const std::string& confirm_new_pw*/) 
{}
////////////////////
////////////////////
bool neroshop::Validator::validate_username(const std::string& username) 
{
    // username (will appear only in lower-case letters within the app)
    // make sure username is at least 2 characters short (min_user_length=2)
    unsigned int min_user_length = 2;
    if(username.length() < min_user_length) {
        NEROSHOP_TAG_OUT std::cout << "Your username must be at least " << min_user_length << " characters in length" << std::endl;
        return false; // exit function
    }
    // make sure username is at least 30 characters long (max_user_length=30)
    unsigned int max_user_length = 30; // what if a company has a long name? :o // also consider the textbox's max-length
    if(username.length() > max_user_length) {
        NEROSHOP_TAG_OUT std::cout << "Your username must be at least " << max_user_length << " characters in length" << std::endl;
        return false; // exit function
    }
    for(int i = 0; i < username.length(); i++)
    {
        // make sure username does not contain any spaces //std::cout << username[i] << " - char\n";
        if(isspace(username[i])) {
            NEROSHOP_TAG_OUT std::cout << "Your username cannot contain any spaces" << std::endl;
            return false; // exit function
        }
        // make sure username can only contain letters, numbers, and these specific symbols: a hyphen, underscore, and period (-,_,.) //https://stackoverflow.com/questions/39819830/what-are-the-allowed-character-in-snapchat-username#comment97381763_41959421
        // symbols like @,#,$,etc. are invalid
        if(!isalnum(username[i])) {
            if(username[i] != '-'){
            if(username[i] != '_'){
            if(username[i] != '.'){
                NEROSHOP_TAG_OUT std::cout << "Your username cannot contain any symbols except (-, _, .): " << username[i] << std::endl;
                return false; // exit function
            }}}
        }
    }
    // make sure username begins with a letter (username cannot start with a symbol or number)
    char first_char = username.at(username.find_first_of(username));
    if(!isalpha(first_char)) {
        NEROSHOP_TAG_OUT std::cout << "Your username must begin with a letter: " << first_char << std::endl;
        return false; // exit function
    }
    // make sure username does not end with a symbol (username can end with a number, but NOT with a symbol)
    char last_char = username.at(username.find_last_of(username));
    if(!isalnum(last_char)) { //if(last_char != '_') { // underscore allowed at end of username? :o
        NEROSHOP_TAG_OUT std::cout << "Your username must end with a letter or number: " << last_char << std::endl;
        return false; // exit function //}
    }
    // the name guest is reserved for guests only, so it cannot be used by any other user
    if(String::lower(username) == "guest") {
        neroshop::print("The name \"Guest\" is reserved for guests ONLY");
        return false;
    }

    return true;    
}
////////////////////
bool neroshop::Validator::validate_password(const std::string& password) 
{
    //Minimum eight characters, at least one uppercase letter, one lowercase letter, one number and one special character:
    const std::regex pattern("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$"); // source: https://stackoverflow.com/questions/19605150/regex-for-password-must-contain-at-least-eight-characters-at-least-one-number-a
    if(!std::regex_match(password, pattern))
    {
        // figure out the specific reason why password failed to follow the regex rules  //if(password.empty()) { std::cout << "Please enter a valid password" << std::endl; return false; }
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[A-Z])"))) {
            neroshop::print("Password must have at least one upper case letter", 1);
        }
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[a-z])"))) {
            neroshop::print("Password must have at least one lower case letter", 1);
        }
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[0-9])"))) {
            neroshop::print("Password must have at least one digit", 1);
        }              
        if(!std::regex_search(password.c_str(), std::regex("(?=.*?[#?!@$%^&*-])"))) {
            neroshop::print("Password must have at least one special character", 1);
        }         
        if(password.length() < 8) {
            neroshop::print("Password must be at least 8 characters long", 1);
        } 
        //neroshop::print("Please enter a stronger password (at least 1 upper case letter, 1 lower case letter, 1 digit, and 1 special character)", 1);
        return false; // exit function
    }
    return true; // default return value
}
////////////////////
bool neroshop::Validator::validate_email(const std::string& email) {
    // make sure email is a valid email
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if(!std::regex_match(email, pattern)) {
        neroshop::print("Email address is not valid", 1);
        return false;
    }
    // check db to see if email is not already taken (last thing to worry about)
	// get email_hash (from email)
	std::string email_hash;
	if(!generate_sha256_hash(email, email_hash)) {
	    neroshop::print("Failed to generate SHA256 hash", 1);
	    return false;
	}

    return true;    
}
////////////////////
bool neroshop::Validator::validate_bcrypt_hash(const std::string& password, const std::string& hash)
{   // verify password
    int result = bcrypt_checkpw(password.c_str(), hash.c_str());
#ifdef NEROSHOP_DEBUG
	NEROSHOP_TAG_OUT std::cout << ((result != 0) ? "\033[0;91mThe password does NOT match" : "\033[0;32mThe password matches") << "\033[0m" << std::endl;
#endif
    return (result == 0);
}
////////////////////
bool neroshop::Validator::generate_bcrypt_salt(unsigned int workfactor, char salt[BCRYPT_HASHSIZE])
{   // generate a salt (random)
    int result = bcrypt_gensalt(workfactor, salt); // workfactor must be between 4 and 31 - default is 12
#ifdef NEROSHOP_DEBUG0
    std::cout << ((result != 0) ? NEROSHOP_TAG "bcrypt salt failed to generate" : NEROSHOP_TAG "generated bcrypt salt: " + std::string(salt)) << std::endl; // 0=success
    // two users with the same password results in the same exact hash, which is why a unique salt must be generated
#endif
    return (result == 0);
}
////////////////////
bool neroshop::Validator::generate_bcrypt_hash(const std::string& password, const char salt[BCRYPT_HASHSIZE], char hash[BCRYPT_HASHSIZE])
{   // generate hash (from password and salt combination)
    int result = bcrypt_hashpw(password.c_str(), salt, hash);
#ifdef NEROSHOP_DEBUG0
    std::cout << ((result != 0) ? NEROSHOP_TAG "bcrypt hash failed to generate" : NEROSHOP_TAG "generated bcrypt hash: " + std::string(hash)) << std::endl;
#endif
    return (result == 0);
}
////////////////////
bool neroshop::Validator::validate_sha256_hash(const std::string& email, const std::string& hash) { // raw/unsalted hash
    std::string temp_hash;
    if(!generate_sha256_hash(email, temp_hash)) return false;
    return (temp_hash == hash);
}
////////////////////
bool neroshop::Validator::generate_sha256_hash(const std::string& email, std::string& hash_out) {
    // EVP (recommended over legacy "SHA256_" functions which are deprecated in OpenSSL 3.0)
    EVP_MD_CTX * context = EVP_MD_CTX_new();
    if(context == nullptr) { neroshop::print("EVP_MD_CTX_new failed", 1); return false; }
    // initialize
    if(EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        neroshop::print(ERR_error_string(ERR_get_error(), nullptr), 1);
        return false;
    }
    // update (generate hash)
    if(EVP_DigestUpdate(context, email.c_str(), email.length()) != 1) {
        EVP_MD_CTX_free(context);
        neroshop::print(ERR_error_string(ERR_get_error(), nullptr), 1);
        return false;
    }
    // finalize
    unsigned char sha256_hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    if(EVP_DigestFinal_ex(context, sha256_hash, &hash_len) != 1) {
        EVP_MD_CTX_free(context);
        neroshop::print(ERR_error_string(ERR_get_error(), nullptr), 1);
        return false;
    }
    // store hash in string
    std::stringstream ss;
    for(unsigned int i = 0; i < hash_len; ++i) 
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)sha256_hash[i];
    hash_out = ss.str();
#ifdef NEROSHOP_DEBUG0
    if(!email.empty()) neroshop::print("generated sha256 hash: " + hash_out + " (" + email + ")");
#endif    
    // if everything went well, free the context and return true
    EVP_MD_CTX_free(context); // renamed from "EVP_MD_CTX_destroy" in 1.1.0, same with EVP_MD_CTX_create => EVP_MD_CTX_new
    return true;
}
////////////////////
////////////////////
////////////////////
////////////////////
