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

        m_server.set_open_handler(bind(&Chatserver::on_open,this,::_1));
        m_server.set_close_handler(bind(&Chatserver::on_close,this,::_1));
        m_server.set_message_handler(bind(&Chatserver::on_message,this,::_1,::_2));
    }

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
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

    void run(uint16_t port) {
        websocketpp::lib::thread command_prompt(&Chatserver::open_command_prompt, this);
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
        command_prompt.join();
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
