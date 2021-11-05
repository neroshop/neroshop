#include "../include/wallet.hpp"
////////////////////
////////////////////
// constructors and destructors
////////////////////
Wallet::Wallet() : wallet_obj(nullptr)
{}
////////////////////
Wallet::Wallet(const std::string& file) {
    if(File::exists(file)) { // if file exists
        open(file.substr(0, file.find(".")));
        this->file = file;
        return; // exit function
    }
    if(!File::exists(file)) { // in case the file does not exist
        create(file.substr(0, file.find("."))); // create the wallet file
        this->file = file;
        return; // exit function
    }
}
////////////////////
Wallet::~Wallet() 
{
    //wallet_obj->remove_listener (sync_listener); // sync listener
    //wallet_obj->remove_listener (listener); // tx_output listener  
    close(false);
}
////////////////////
////////////////////
// normal
////////////////////
void Wallet::connect() {
    // connect a hardware wallet
}
////////////////////
void Wallet::create(std::string password/*, const std::string &confirm_pw*/, monero_network_type network_type) {
    std::string path = (!file.empty()) ? file.substr(0, file.find(".")) : "wallet";// will create a wallet.keys file in the current path
    wallet_obj = monero_wallet_full::create_wallet_random (path, password, network_type/*, const monero_rpc_connection &daemon_connection=monero_rpc_connection(), const std::string &language="English", std::unique_ptr< epee::net_utils::http::http_client_factory > http_client_factory=nullptr*/);
    if(wallet_obj) std::cout << "\033[1;35;49m" << "created wallet \"" << path << ".keys\"" << "\033[0m" << std::endl;
}
////////////////////
void Wallet::create_from_mnemonic(const std::string& mnemonic, std::string password/*, const std::string &confirm_pw*/, monero_network_type network_type) {
    // retrieve keys from text edit
    std::string path = (!file.empty()) ? file.substr(0, file.find(".")) : "wallet";// will create a wallet.keys file in the current path
    wallet_obj = monero_wallet_full::create_wallet_from_mnemonic (path, password, monero_network_type::MAINNET, mnemonic/*, const monero_rpc_connection &daemon_connection=monero_rpc_connection(), uint64_t restore_height=0, const std::string &seed_offset="", std::unique_ptr< epee::net_utils::http::http_client_factory > http_client_factory=nullptr*/);
    if(wallet_obj) std::cout << "\033[1;35;49m" << "created wallet \"" << path << ".keys\" (from mnemonic)" << "\033[0m" << std::endl;
}
////////////////////
void Wallet::create_from_keys(const std::string& address, const std::string& view_key, const std::string& spend_key, std::string password/*, const std::string &confirm_pw*/, monero_network_type network_type) {
    // retrieve keys from text edit
    std::string path = (!file.empty()) ? file.substr(0, file.find(".")) : "wallet";// will create a wallet.keys file in the current path
    wallet_obj = monero_wallet_full::create_wallet_from_keys (path, password, network_type, address, view_key, spend_key/*, const monero_rpc_connection &daemon_connection=monero_rpc_connection(), uint64_t restore_height=0, const std::string &language="English", std::unique_ptr< epee::net_utils::http::http_client_factory > http_client_factory=nullptr*/);
    if(wallet_obj) std::cout << "\033[1;35;49m" << "created wallet \"" << path << ".keys\" (from keys)" << "\033[0m" << std::endl;
}
////////////////////
std::string Wallet::upload(bool open, std::string password, monero_network_type network_type) { // opens the wallet file
    // open file dialog to retrieve walletfile path
    // reminder: use this function for an upload button instead
    //           then call Wallet::open when user presses a "submit" button.
#ifdef __gnu_linux__
    char file[1024];
    FILE *f = popen("zenity --file-selection", "r");
    fgets(file, 1024, f);
#endif
    std::string filename(file); // "wallet.keys" file
    filename = filename.substr(0, filename.find(".")); // remove ext
    if(!File::exists(filename + ".keys")) {std::cout << "\033[1;31;49m" << "wallet not found" << "\033[0m" << std::endl; return "";}// check if wallet file is valid (or exists)
    if(open == true) Wallet::open(filename, password, network_type);// will re-apply ".keys" ext to the wallet file // check if wallet exists first//monero::monero_wallet_full::wallet_exists	(	const std::string & 	path	)	
    return std::string(filename + ".keys");
}
////////////////////
void Wallet::open(const std::string& path, std::string password, monero_network_type network_type) { // opens the wallet file's name without the ".key" extension
    wallet_obj = monero::monero_wallet_full::open_wallet(path, password, network_type); // will re-apply ".keys" ext to the wallet file
    if(wallet_obj) std::cout << "\033[1;35;49m" << "opened wallet \"" << path << ".keys\"" << "\033[0m" << std::endl;
}
////////////////////
void Wallet::restore(const std::string& mnemonic, std::string password, monero_network_type network_type) 
{}
////////////////////
void Wallet::restore(const std::string& address, const std::string& view_key, const std::string& spend_key, std::string password, monero_network_type network_type)
{}
////////////////////
void Wallet::close(bool save) 
{
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    wallet_obj->close(save);
}
////////////////////
void Wallet::transfer(const std::string& address, double amount) {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    //Configures a transaction to send, sweep, or create a payment URI.
    // send funds from the restored wallet to the random wallet
    monero_tx_config config;
    config.m_account_index = 0; // withdraw funds from account at index 0
    config.m_address = address; // wallet_random // boost::optional< std::string > 	m_address
    config.m_amount = amount; // boost::optional< uint64_t > 	m_amount
    config.m_relay = true;
    std::shared_ptr<monero_tx_wallet> sent_tx = wallet_obj->create_tx(config);
    bool in_pool = sent_tx->m_in_tx_pool.get();  // true
    
    //uint64_t fee = sent_tx->m_fee.get(); // "Are you sure you want to send ...?"
    //wallet_obj->relay_tx(*sent_tx); // recipient receives notification within 5 seconds    
    // prove that you've sent the payment using "get_tx_proof"
} // "transfer"
////////////////////
////////////////////
void Wallet::sweep_all(const std::string& address) {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    //std::vector< std::shared_ptr< monero_tx_wallet > > monero::monero_wallet_full::sweep_dust 	( 	bool  	relay = false	) 	
} // sends entire balance, including dust to an address // "sweep_all <address>"
////////////////////
////////////////////
std::string Wallet::address_new() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // Create a subaddress within an account (account_0 to be specific).
    monero_subaddress subaddress = wallet_obj->create_subaddress(0);//monero_subaddress monero::monero_wallet_full::create_subaddress	(	uint32_t 	account_idx,const std::string & 	label = "" )
#ifdef NEROSHOP_DEBUG0
    std::cout << "address_new: " << subaddress.m_address.get() << " (account_idx: " << subaddress.m_account_index.get() << ", subaddress_idx: " << subaddress.m_index.get() << ")" << std::endl;
#endif
    //if subaddress has already been used
    if(subaddress.m_is_used.get()) { std::cout << "subaddress already in use!" << std::endl; return ""; }
    return subaddress.m_address.get();// return the newly created subaddress
    // store new subaddress//subaddress_list.push_back(subaddress.m_address.get());
} // generates a new subaddress from main account // "address new"
////////////////////
void Wallet::address_book_add(const std::string& address, std::string description) {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    wallet_obj->add_address_book_entry(address, description);//unsigned int index = monero::monero_wallet_full::add_address_book_entry	(	address, const std::string & 	description );
}
////////////////////
void Wallet::address_book_delete(unsigned int index) {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    wallet_obj->delete_address_book_entry(index);//void monero::monero_wallet_full::delete_address_book_entry	(	uint64_t 	index	);
}
////////////////////
//void  Wallet::explore(const std::string& address) {} // will detect address before opening explorer
////////////////////
std::vector<std::string> Wallet::address_all() 
{
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    if(!wallet_obj->is_synced()) throw std::runtime_error("wallet is not synced with a daemon");
    std::vector<std::string> address_list;
    std::vector<unsigned int> subaddress_indices;
    for(int i = 0; i < wallet_obj->get_account(0, true).m_subaddresses.size(); i++) {
        std::vector<monero_subaddress> 	subaddresses = wallet_obj->get_subaddresses(0, subaddress_indices); // retrieve subaddress from account_0
#ifdef NEROSHOP_DEBUG0
        std::cout << subaddresses[i].m_index.get() << " " << subaddresses[i].m_address.get() << (subaddresses[i].m_is_used.get() ? " (used)" : "") << std::endl;//std::cout << "account(" << subaddresses[i].m_account_index.get() << "), subaddress(" << subaddresses[i].m_index.get() << "): " << subaddresses[i].m_address.get() << std::endl;//std::cout << "account(" << wallet_obj->get_account(0, true).m_index.get() << "), subaddress(" << wallet_obj->get_account(0, true).m_subaddresses[i].m_index.get() << "): " << wallet_obj->get_account(0, true).m_subaddresses[i].m_address.get() << std::endl;
#endif
        address_list.push_back(subaddresses[i].m_address.get()); // store all addresses in vector - bad idea as it can store duplicate addresses
    }
    return address_list;
}
////////////////////
std::vector<std::string> Wallet::address_used() 
{
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    if(!wallet_obj->is_synced()) throw std::runtime_error("wallet is not synced with a daemon");
    std::vector<std::string> address_list;
    std::vector<unsigned int> subaddress_indices;
    for(int i = 0; i < wallet_obj->get_account(0, true).m_subaddresses.size(); i++) {
        std::vector<monero_subaddress> 	subaddresses = wallet_obj->get_subaddresses(0, subaddress_indices); // retrieve subaddress from account_0
        if(subaddresses[i].m_is_used.get()) {
#ifdef NEROSHOP_DEBUG0        
            std::cout << subaddresses[i].m_index.get() << " " << subaddresses[i].m_address.get() << std::endl;
#endif            
            address_list.push_back(subaddresses[i].m_address.get()); // store used addresses in vector
        }
    }
    return address_list;
}
////////////////////
std::vector<std::string> Wallet::address_unused() 
{
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    if(!wallet_obj->is_synced()) throw std::runtime_error("wallet is not synced with a daemon");
    std::vector<std::string> address_list; // local std::vector
    std::vector<unsigned int> subaddress_indices;
    for(int i = 0; i < wallet_obj->get_account(0, true).m_subaddresses.size(); i++) {
        std::vector<monero_subaddress> 	subaddresses = wallet_obj->get_subaddresses(0, subaddress_indices); // retrieve subaddress from account_0
        if(!subaddresses[i].m_is_used.get()) {
#ifdef NEROSHOP_DEBUG
            std::cout << subaddresses[i].m_index.get() << " " << subaddresses[i].m_address.get() << std::endl;
#endif            
            address_list.push_back(subaddresses[i].m_address.get()); // store unused addresses in vector
        }
    }
    return address_list;
}
////////////////////
// open the daemon before opening the wallet
void Wallet::daemon_open(const std::string& ip, const std::string& port, bool confirm_external_bind, bool restricted_rpc, bool remote, std::string data_dir, std::string network_type, unsigned int restore_height) 
{
    // search for monero daemon (that is packaged with neroshop executable)
    std::string daemon_dir = File::get_current_dir() + "/external/monero-cpp/external/monero-project/build/release/bin/monerod";
    // connect to a remote node
    // either neroshop's daemon does not exist or remote is set to true
    if(!File::exists(daemon_dir) || remote == true) { std::cout << "\033[1;90;49m" << "connecting to remote node - " << (ip + ":" + port) << "\033[0m" << std::endl; return;} // exit function
    // check if there is another monerod process running in the background
    int monerod = Process::get_process_by_name("monerod");//(argv[1]);//cout << "pid: " << monerod << endl;
    if(monerod != -1) { std::cout << "\033[1;90;49m" << "monerod is running (ID:" << monerod << ")\033[0m" << std::endl; return;} // daemon that was previously running in the background // exit function
    // if neroshop's daemon exists and remote is set to false //if(File::exists(daemon_dir) && remote == false) {  
	// if no other daemon is running, then use daemon that comes packaged with neroshop executable
	//if(monerod == -1) {
    std::cout << "\033[1;90;49m" << "daemon found: \"" << daemon_dir << "\"" << "\033[0m" << std::endl;
    std::string program  = daemon_dir;
    std::string args = (" --data-dir=" + data_dir) + (" --rpc-bind-ip=" + ip) + (" --rpc-bind-port=" + port);
    if(ip == "0.0.0.0" && confirm_external_bind == true) { args = args + " --confirm-external-bind"; }
    if(confirm_external_bind == true && restricted_rpc == true) { args = args + " --restricted-rpc"; }
    if(String::lower(network_type) != "mainnet") args = args + (" --" + network_type);
    args = args + (" --detach"); // https://monero.stackexchange.com/questions/12005/what-is-the-difference-between-monerod-detach-and-monerod-non-interactive
    // start the daemon (monerod) as a new process on launch  //std::cout << "\033[1;95;49m" << "$ " << daemon_dir + args << "\033[0m" << std::endl;
	process = new Process(daemon_dir, args); // note: process was being created on the stack, not the heap so it died in the function scope/if statement
    monerod = process->get_handle();
    std::cout << "\033[1;90;49m" << "started monerod (ID:" << monerod << ")\033[0m" << std::endl;
    //}//}
}
////////////////////
Progressbar * Wallet::sync_bar (nullptr);
////////////////////
Label * Wallet::sync_label (nullptr);
////////////////////
////////////////////
// create wallet_listener to synchronize the wallet and receive progress notifications
struct : monero_wallet_listener { // listener	- listener to receive notifications during synchronization 
    void on_sync_progress(uint64_t height, uint64_t start_height, uint64_t end_height, double percent_done, const std::string& message) {
	    /*auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now); // current time
	    std::stringstream ss;
	    ss << std::put_time(std::localtime(&in_time_t), "[%Y-%m-%d  %l:%M:%S %p]");
	    std::string date = ss.str();
        std::cout << "\033[0;35;49m" << date << " \033[1;33;49m" << "**********************************************************************" << "\033[0m" << std::endl;
        std::cout << "\033[0;35;49m" << date << " \033[1;33;49m" << "Synced " << height << "/" << end_height;//<< "\033[0m" << std::endl;
        unsigned int blocks_to_sync = end_height - height;
        std::cout << "\033[1;33;49m" << " [Your node is " << blocks_to_sync << " block(s) behind]" << "\033[0m" << std::endl;//" blocks (" << blocks_to_sync / 60? << " minutes) behind]" << "\033[0m" << std::endl; // 1 block = 1 minute
        std::cout << "\033[0;35;49m" << date << " \033[1;33;49m" << message << ": " << (percent_done * 100) << "%" << "\033[0m" << std::endl;
        if((percent_done * 100) == 100) std::cout << "\033[0;35;49m" << date << " \033[1;32;49m" << "SYNCHRONIZATION DONE" << std::endl;
        std::cout << "\033[0;35;49m" << date << std::endl;
        std::cout << "\033[0;35;49m" << date << " \033[1;33;49m" << "**********************************************************************" << "\033[0m" << std::endl;*/
        ///////////////////////////////////
        if(!Wallet::sync_bar) {
            Wallet::sync_bar = new Progressbar();//sync_bar->set_range(0.0, 100.0); // 0-100%//sync_bar->set_outline(true);
            Wallet::sync_bar->set_size(300, 30);
            Wallet::sync_label = new Label();
            Wallet::sync_label->set_alignment("center");
            Wallet::sync_bar->set_label(*Wallet::sync_label);
        }
        WINDOW * window = static_cast<WINDOW *>(Factory::get_window_factory()->get_object(0)); // using WINDOW::get_active() causes a seg fault when window loses focus while syncing - since it only gets the window that has focus :(
        window->set_viewport(Renderer::get_display_size().x, Renderer::get_display_size().y);//(1280, 720);
        window->clear(32, 32, 32);
        double progress = percent_done * 100; //sync_bar->set_range(0, 100);
        //////////
        if(progress < 100) { // while progress is not 100%, send a fake event to keep the progressbar going
        #ifdef __gnu_linux__ 
        	XKeyEvent event_send;
            event_send.type = KeyPress;
            XSendEvent(window->get_display(), window->get_handle(), true, KeyPressMask, (XEvent *)&event_send);// ...
        #endif    
        }
        //////////        
        Wallet::sync_bar->get_label()->set_string(std::to_string(progress) + "%");
        Wallet::sync_bar->set_value(progress);
        Wallet::sync_bar->set_position((window->get_client_width() / 2) - (Wallet::sync_bar->get_width() / 2), (window->get_client_height() / 2) - (Wallet::sync_bar->get_height() / 2));// center progressbar
        Wallet::sync_bar->draw();
        if(progress == 100) {neroshop::Message("SYNCHRONIZATION DONE", 107,142,35); neroshop::Message::show(); neroshop::Message::draw();}
        window->update();
    }
} sync_listener;
////////////////////
struct : monero_wallet_listener {
    //void on_new_block (uint64_t height) {}
    void on_output_received(const monero_output_wallet& output) {
        uint64_t amount = output.m_amount.get();
        std::string tx_hash = output.m_tx->m_hash.get();
        bool is_confirmed = output.m_tx->m_is_confirmed.get(); // unlocked_balance (can be spent)
        bool is_locked = std::dynamic_pointer_cast<monero_tx_wallet>(output.m_tx)->m_is_locked.get(); // balance (locked - still processing)
        int account_index = output.m_account_index.get();
        int subaddress_index = output.m_subaddress_index.get();
        // get balance (locked)
        double piconero = 0.000000000001;
        double balance = (amount * piconero);
        // you've received some xmr but it may be unspendable for the meantime
        if(is_locked) {
            std::cout << "\033[1;32;49m" << "You have received " << std::fixed << std::setprecision(12) << balance << std::fixed << std::setprecision(2) << " xmr "/* << subaddress.m_address.get()*/ << "(txid: " << tx_hash << ", account_idx: " << account_index << ", subaddress_idx: " << subaddress_index << ")" << "\033[0m" << std::endl;
            //std::cout << "\033[1;32;49m" << "txid <" << tx_hash << ">, " << std::fixed << std::setprecision(12) << (amount * piconero) << std::fixed << std::setprecision(2) << ", idx (" << account_index << ", " << subaddress_index << ")" << "\033[0m" << std::endl;
            neroshop::Message(std::string("output received: " + std::to_string(balance) + " xmr"), 34, 139, 34);
            neroshop::Message::show();
        }
    }
    void on_balances_changed (uint64_t new_balance, uint64_t new_unlocked_balance) {
        double piconero = 0.000000000001;
        double balance = (new_balance * piconero);
        double unlocked_balance = (new_unlocked_balance * piconero);
        // if total balance is still locked, display it
        // but if total balance is fully unlocked, then you already have the balance so no need to keep displaying it 
        if(unlocked_balance != balance) {
            // balance updated (unlocked)
            std::cout << std::fixed << std::setprecision(12) << "\033[1;33;49m" << "balance: " << "\033[0m" << balance << std::fixed << std::setprecision(2);// << std::endl;
            std::cout << std::fixed << std::setprecision(12) << " (unlocked_balance: " << unlocked_balance << std::fixed << std::setprecision(2) << ")" << std::endl;
        }
        if(unlocked_balance == balance) {
            std::cout << "\033[1;35;49m" << "Balance is now fully unlocked" << "\033[0m" << std::endl;// Your full balance can be spent now
            //std::cout << std::fixed << std::setprecision(12) << "balance: " << balance << std::fixed << std::setprecision(2);// << std::endl;
            //std::cout << std::fixed << std::setprecision(12) << " (unlocked_balance: " << unlocked_balance << std::fixed << std::setprecision(2) << ")" << std::endl;
        }
    }
} wallet_listener;
////////////////////
////////////////////
bool Wallet::daemon_connect(const std::string& ip, const std::string& port) { // connect to a running daemon (node)
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // connect to the daemon
	wallet_obj->set_daemon_connection(monero_rpc_connection(std::string("http://" + ip + ":" + port)));//, string("superuser"), string("abctesting123")));
    std::cout << "\033[1;90;49m" << "waiting for daemon" << "\033[0m" << std::endl;
    bool connected = false; bool synced = false;
    while(!connected) {
        if(wallet_obj->is_connected_to_daemon()) {
            std::cout << "\033[1;90;49m" << "connected to daemon" << "\033[0m" << std::endl; // connected to a daemon but it may not be fully synced with the network
            connected = true;
            // when connected to daemon, listen to sync progress (use this only on a detached daemon)
            std::cout << "\033[1;90;49m" << "sync in progress .." << "\033[0m" << std::endl;
            // it is not safe to connect to a daemon that has not fully synced, so listen to the sync progress before attempting to do anything else
            wallet_obj->sync(sync_listener);//monero_sync_result sync_result = wallet_obj->sync(sync_listener); // synchronize the wallet with the daemon as a one-time synchronous process//if(sync_result.m_received_money) {neroshop::print(std::string("blocks fetched: ") + std::to_string(sync_result.m_num_blocks_fetched));neroshop::print("you have received money");}
            wallet_obj->remove_listener(sync_listener); // remove sync_listener, since we are done getting the sync progress           
            // continue syncing in order to receive tx notifications
            wallet_obj->start_syncing(5000); // begin syncing the wallet constantly inside the background
            // check if wallet's daemon is synced with the network
            if(wallet_obj->is_daemon_synced()) synced = true;//{std::cout << "\033[1;90;49m" << "daemon is now fully synced with the network" << "\033[0m" << std::endl;synced = true;}
        }
    }
    wallet_obj->add_listener(wallet_listener); // add wallet_listener
    return synced;
}
////////////////////
void Wallet::daemon_close() {
    if(!process) throw std::runtime_error("process is not initialized");
#ifdef __gnu_linux__
    int monerod = process->get_handle();
    if(monerod != -1) kill(static_cast<pid_t>(monerod), SIGTERM);
#endif    
}
////////////////////
////////////////////
void Wallet::wallet_info() {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // wallet must be synced with a daemon in order to get the *most recent* wallet info
    if(!wallet_obj->is_synced()) throw std::runtime_error("wallet is not synced with a daemon");
    // get primary_address, balance, unlocked_balance, etc.
    std::string restored_primary = wallet_obj->get_primary_address(); // same as: wallet_obj->get_account(0, true).m_primary_address.get()
    std::cout << "primary_address: " << restored_primary << std::endl;
    uint64_t balance_raw = wallet_obj->get_balance();    // can specify account and subaddress indices
    double piconero = 0.000000000001; // the smallest unit of a monero (monero has 12 decimal places) // https://web.getmonero.org/resources/moneropedia/denominations.html
    double balance = (double)balance_raw * piconero;
    std::cout << std::fixed << std::setprecision(12) << "balance: " << balance << std::fixed << std::setprecision(2) << std::endl;
    // account ---------------------------------------------------------------
    // account_0 is the default/main account where the primary address derives from
    monero_account account = wallet_obj->get_account(0, true);       // get account with subaddresses
    uint64_t unlocked_balance_raw = account.m_unlocked_balance.get();
    double unlocked_balance = (double)unlocked_balance_raw * piconero;
    std::cout << std::fixed << std::setprecision(12) << "unlocked_balance: " << unlocked_balance << std::fixed << std::setprecision(2) << std::endl; // uint64_t
    // view and spend keys ----------------------------------------------------
    std::cout << "view_key: " << get_viewkey("supersecretpassword123").first << std::endl;
    std::cout << "spend_key: " << "(secret)" << std::endl; // since this is intended to be a view-only wallet
    // subaddress -------------------------------------------------------------
    // generate new subaddresses on the default account 0
    std::cout << std::endl;
    /*for(int i = 0; i < 10; i++) wallet->address_new();
    // get number of subaddresses
    std::cout << std::endl;
    std::cout << "address count: " << wallet->get_address_count() << std::endl;
    std::string last_subaddress = wallet->get_last_subaddress();
    std::cout << "last subaddress created: " << last_subaddress << std::endl;*/
}
////////////////////
////////////////////
// setters
////////////////////
void Wallet::set_tx_note(const std::string& txid, const std::string& tx_note) {} // "set_tx_note <txid> [free note text here]" - useful for filling address information
////////////////////
////////////////////
// getters
////////////////////
std::string Wallet::get_primary_address() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return wallet_obj->get_primary_address();// same as://wallet_obj->get_account(0, true).m_primary_address.get();//monero_account monero::monero_wallet_full::get_account	(	const uint32_t 	account_idx,bool 	include_subaddresses )		//const
}
////////////////////
std::string Wallet::get_address(unsigned int index) const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return wallet_obj->get_account(0, true).m_subaddresses[index].m_address.get(); // account_idx "0" is default
}
////////////////////
unsigned int Wallet::get_address_count() const {  
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return wallet_obj->get_account(0, true).m_subaddresses.size();// all subaddresses will be created on the default account at index 0
}
//////////////////// for some reason, "account.m_subaddresses[i].m_address.get()" fails, but "wallet_obj->get_account(0, true).m_subaddresses.size()" usually succeeds ...-> https://stackoverflow.com/questions/68733975/difficult-to-understand-runtime-error-this-is-initialized-failure-in-boost#comment121472560_68733975
double Wallet::get_balance_raw(unsigned int account_index, unsigned int subaddress_index) const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // get balance from subaddress
    if(subaddress_index > 0) return wallet_obj->get_balance(account_index, subaddress_index);//uint64_t monero::monero_wallet_full::get_balance	(	uint32_t 	account_idx, uint32_t 	subaddress_idx )		//const
    // get balance from account (primary address)
    else return wallet_obj->get_balance(account_index);//uint64_t monero::monero_wallet_full::get_balance	(	uint32_t 	account_idx	)	const
    return 0.0;
} // "balance"
////////////////////
double Wallet::get_unlocked_balance_raw(unsigned int account_index, unsigned int subaddress_index) const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // get a subaddress's unlocked balance
    if(subaddress_index > 0) return wallet_obj->get_unlocked_balance(account_index, subaddress_index);//uint64_t monero::monero_wallet_full::get_unlocked_balance	(	uint32_t 	account_idx, uint32_t 	subaddress_idx )		const
    // get an account's unlocked balance (primary address)
    else return wallet_obj->get_unlocked_balance(account_index);//uint64_t monero::monero_wallet_full::get_unlocked_balance	(		)	const//uint64_t monero::monero_wallet_full::get_unlocked_balance	(	uint32_t 	account_idx	)	const
    return 0.0;
} // "balance"
////////////////////
double Wallet::get_balance(unsigned int account_index, unsigned int subaddress_index) const {
    double piconero = 0.000000000001;
    return get_balance_raw(account_index, subaddress_index) * piconero;
}
////////////////////
double Wallet::get_unlocked_balance(unsigned int account_index, unsigned int subaddress_index) const {
    double piconero = 0.000000000001;
    return get_unlocked_balance_raw(account_index, subaddress_index) * piconero;
}
////////////////////
std::vector<std::string> Wallet::get_transactions() const {std::vector<std::string> vstr;return vstr;} // "show_transfers"
////////////////////
unsigned int Wallet::get_transactions_count() const {return 0;}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
std::string Wallet::get_last_subaddress() const 
{
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // unless wallet is synced to a daemon, you will not be able to get the *most recent* last subaddress created
    if(!wallet_obj->is_synced()) throw std::runtime_error("wallet is not synced with a daemon");
    unsigned int last = wallet_obj->get_account(0, true).m_subaddresses.size() - 1;
    return wallet_obj->get_account(0, true).m_subaddresses[last].m_address.get();
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// proof (proving the transaction was submitted) - https://web.getmonero.org/resources/user-guides/prove-payment.html
std::string  Wallet::get_tx_note(const std::string& txid) const {return "";} // "get_tx_note <txid>" - useful for retrieving address information
////////////////////
std::pair<std::string, std::string> Wallet::get_viewkey(const std::string& password) const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return std::make_pair(wallet_obj->get_private_view_key(), wallet_obj->get_public_view_key());
} // secret, public // "viewkey"
////////////////////
std::pair<std::string, std::string> Wallet::get_spendkey(const std::string& password) const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return std::make_pair(wallet_obj->get_private_spend_key(), wallet_obj->get_public_spend_key());
} // secret, public // "spendkey"
////////////////////
std::string  Wallet::get_mnemonic(const std::string& password) const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return wallet_obj->get_mnemonic();
} // "seed"
//-Image *  Wallet::get_qr_code() const {} // returns address qrcode // "show_qr_code"
//-Image *  Wallet::get_qr_code(unsigned int address_index) const {} // returns the qrcode of the address at "index"
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
std::string  Wallet::get_file() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    // Get the path of this wallet's file on disk.
    return wallet_obj->monero_wallet_full::get_path();//	const
} // "wallet_info"
////////////////////
std::string  Wallet::get_description() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return "";
} // "wallet_info"    
////////////////////
std::string  Wallet::get_type() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return "";
} // "wallet_info": Normal, HW
////////////////////
std::string  Wallet::get_network_type() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    //monero_network_type monero::monero_wallet_full::get_network_type	(		)	const
    return "";
} // "wallet_info":  Mainnet, Testnet, Stagenet
////////////////////
std::string  Wallet::get_status() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    return "";
} // "status" - Check current status of wallet.
////////////////////
std::string  Wallet::get_version() const {
    if(!wallet_obj) throw std::runtime_error("monero_wallet_full is not opened");
    //monero_version monero::monero_wallet_full::get_version	(		)	const
    return "";
} // "version" - Check software version.
////////////////////
monero_wallet_full * Wallet::get_monero_wallet() const
{
    return wallet_obj;
}
////////////////////
// callbacks
////////////////////
void Wallet::load_from_config(std::string/*const std::string&*/ password) // load configs on opening
{

}
////////////////////
////////////////////
//void  Wallet::set_daemon() {} // "set_daemon <host>[:<port>] [trusted|untrusted|this-is-probably-a-spy-node]" - connects to a daemon
////////////////////
//void  Wallet::refresh() {}
////////////////////
void Wallet::config() {
    // make sure config file exists
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////

////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
// void monero::monero_wallet_full::rescan_blockchain	(		) // dangerous! restarts blockchain sync!
// get last subaddress created
//unsigned int last = (wallet_obj->get_account(0, true).m_subaddresses.size() - 1);
//return wallet_obj->get_account(0, true).m_subaddresses[last].m_address.get() << std::endl;
//wallet_obj->move_to (wallet_file, "supersecretpassword123"); // Move the wallet from its current path to the given path.
//std::cout << "\033[1;97;49m" << "moved file \"" << wallet->get_file() << "\" to \"" << wallet_file << "\"\033[0m" << std::endl;
