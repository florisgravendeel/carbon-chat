#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class Chatserver {
public:
    Chatserver() {
        m_server.init_asio();

        m_server.set_access_channels(websocketpp::log::alevel::all);
        m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        m_server.set_open_handler(bind(&Chatserver::on_successful_new_connection,this,::_1));
        m_server.set_close_handler(bind(&Chatserver::on_close_connection,this,::_1));
        m_server.set_fail_handler(bind(&Chatserver::on_connection_failed, this,::_1));
        m_server.set_message_handler(bind(&Chatserver::on_message_received,this,::_1,::_2));
    }

    void on_successful_new_connection(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close_connection(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void on_connection_failed(connection_hdl hdl){
        std::cout << "Connection failed for client." << std::endl;
    }

    void on_message_received(connection_hdl hdl, server::message_ptr msg) {
        if (msg->get_payload() == "User: stopserver"){
            stop();
            return;
        }
        for (auto it : m_connections) {
            m_server.send(it,msg);
        }
    }
    void stop(){
        m_server.stop_listening();
        for (auto it : m_connections) {
            websocketpp::lib::error_code error;
            m_server.close(it, websocketpp::close::status::going_away, "Server shutting down.", error);
            if (error){
                std::cout << "Error stop(): " << error.message() << std::endl;
            }
        }
        m_server.stop();
    }

    void start() {
        websocketpp::lib::thread command_prompt_thread(&Chatserver::open_command_prompt, this);
        websocketpp::lib::thread server_thread(&server::run, &m_server);
        m_server.listen(9002);
        m_server.start_accept();
        m_server.run();
        server_thread.join();
        command_prompt_thread.join();
    }

    void open_command_prompt() {
        std::string input;
        while (true)
        {
            //Read user input from stdin
            std::getline(std::cin, input);
            if (input == "/stop"){
                stop();
                break;
            }
        }
    }
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    server m_server;
    con_list m_connections;
};
