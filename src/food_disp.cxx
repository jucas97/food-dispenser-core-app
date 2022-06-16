#include <iostream>
#include <functional>

#include "food_disp.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

void food_disp::config::parse()
{
    FILE *file;
    rapidjson::Document document;
    char buffer[500];

    file = fopen(CONFIG_FILE, "r");
    rapidjson::FileReadStream input_stream{file, buffer, sizeof(buffer)};
    document.ParseStream(input_stream);

    if (!document.HasMember("host")) {
        std::cout << "Failure, can not proceed without host" << std::endl;
        goto cleanup;
    }
    m_host = document["host"].GetString();

    if (document.HasMember("auth")) {
        for (auto &u_value: document["auth"]["user"].GetArray())
            m_user.push_back(u_value.GetInt());
        for (auto &p_value: document["auth"]["pw"].GetArray())
            m_pw.push_back(p_value.GetInt());
    }

    if (document.HasMember("motor")) {
        m_device = document["motor"]["device"].GetString();
        m_channel = document["motor"]["channel"].GetInt();
        m_period = document["motor"]["period"].GetUint64();
    }

cleanup:
    fclose(file);
}

food_disp::food_disp()
    : m_config()
    , m_mosq(nullptr)
    , m_motor(nullptr)
{
    std::cout << "FD Constructor" << std::endl;
}

food_disp::~food_disp()
{
    std::cout << "FD Destructor" << std::endl;
}

int food_disp::state_machine(steps step)
{
    switch (step) {
        case steps::PARSE_CONFIG:
            m_config.parse();
            break;
        case steps::CONNECT_MQTT: {
            std::vector<std::string> topics{"test"};
            m_mosq = std::make_shared<fd_mosquitto>(topics, m_config.m_host, std::bind(&food_disp::mqtt_message_callback, this, std::placeholders::_1));
            return m_mosq->connect(m_config.m_user, m_config.m_pw);
        }
        case steps::SETUP_MOTOR:
            m_motor = std::make_shared<motor>(m_config.m_channel, m_config.m_period);
            if (m_motor != nullptr) {
                m_motor->open_device(m_config.m_device);
                m_motor->initialize_channel();
            }
            break;
        case steps::LOOP_MQTT:
            m_mosq->loop_forever();
            break;
        default:
            std::cout << "Unknown step" << std::endl;
            break;
    }

    return 0;
}

void food_disp::mqtt_message_callback(uint64_t duty_cycle)
{
    std::cout << "Received duty cycle " << duty_cycle << std::endl;
    if (m_motor != nullptr) {
        m_motor->duty_cycle(duty_cycle);
        m_motor->state(true);
        m_motor->apply_state();
    }
}

int main(int argc, char *argv[])
{
    int rc;
    food_disp app{};

    app.state_machine(steps::PARSE_CONFIG);
    if (app.state_machine(steps::CONNECT_MQTT) != 0) {
        std::cout << "Failed to connect" << std::endl;
        return -1;
    }
    app.state_machine(steps::SETUP_MOTOR);
    app.state_machine(steps::LOOP_MQTT);

    return 0;
}
