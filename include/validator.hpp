// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef VALIDATOR_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define VALIDATOR_HPP_NEROSHOP

#include <iostream>
#include <string>
#include <utility> // std::pair
#include <regex>   // std::regex
#include <cctype>  // isspace, isalpha, isalnum, etc.
#include <fstream> // std::ofstream, std::ifstream
#include <chrono>  // std::chrono
#include <iomanip> //std::put_time, std::setfill, std::setw
// neroshop
#include "db.hpp"
////extern "C" {
// libbcrypt
#include "bcrypt.h"
#include "crypt_blowfish/ow-crypt.h" // used to salt and hash passwords (with bcrypt)
// openssl - monero uses openssl so we just have to link this code to monero
#include <openssl/evp.h> // used to hash email addresses (with SHA-2) for the purpose of hiding it from prying eyes
#include <openssl/err.h>
////}

namespace neroshop {
class Validator {
public:
    static bool register_user(const std::string& username, const std::string& password, const std::string& confirm_pw, std::string opt_email = "");
    static bool login(const std::string& username, const std::string& password);
    static bool login_with_email(const std::string& email, const std::string& password);
    static void save_user(const std::string& username, const char pw_hash[BCRYPT_HASHSIZE], std::string email_hash = "");
    static void change_pw(const std::string& old_pw, const std::string& new_pw/*, const std::string& confirm_new_pw*/);
    static std::string get_date(std::string format="[%Y-%m-%d  %l:%M:%S %p]"); // %H changed to %l
    // boolean
    static bool validate_username(const std::string& username); // for registration
    static bool validate_password(const std::string& password); // for registration
    static bool validate_email(const std::string& email);
    static bool validate_bcrypt_hash(const std::string& password, const std::string& hash); // for login // bycrypt only takes 55-72 character strings // https://security.stackexchange.com/questions/6623/pre-hash-password-before-applying-bcrypt-to-avoid-restricting-password-length
    static bool generate_bcrypt_salt(unsigned int workfactor, char salt[BCRYPT_HASHSIZE]);
    static bool generate_bcrypt_hash(const std::string& password, const char salt[BCRYPT_HASHSIZE], char hash[BCRYPT_HASHSIZE]);
    static bool validate_sha256_hash(const std::string& email, const std::string& hash); // emails do not need to be salted since they are not required for logins
    static bool generate_sha256_hash(const std::string& email, std::string& hash_out);
private:
};
}
#endif
/*
testing speeds:
$ openssl speed sha256 sha512

usernames:
// 2-30 characters in length
// cannot contain any spaces
// cannot contain any symbols except: -, _, and .
// can only begin with a letter
// can only end with a letter or number

passwords:
// at least one upper case English letter, (?=.*?[A-Z])
// at least one lower case English letter, (?=.*?[a-z])
// at least one digit, (?=.*?[0-9])
// at least one special character, (?=.*?[#?!@$%^&*-])
// minimum eight in length .{8,} (with the anchors) 

examples:
	std::string email = "layter@pm.me";
	std::string hash0;
	Validator::generate_sha256_hash("layter@pm.me", hash0);
	std::cout << "sha256_hash (legacy): " << hash0 << std::endl;
	//=> b4055e9e571ea8acf49e937c102901f3f29f50046c9157c4b1849b05bc050bd8
	std::string hash1;
    Validator::generate_sha256_hash_evp(email, hash1);
    std::cout << "sha256_hash (EVP): " << hash1 << std::endl;
    //=> b4055e9e571ea8acf49e937c102901f3f29f50046c9157c4b1849b05bc050bd8
    std::cout << "verify email: " << Validator::validate_sha256_hash(email, "b4055e9e571ea8acf49e937c102901f3f29f50046c9157c4b1849b05bc050bd8") << std::endl;
*/
