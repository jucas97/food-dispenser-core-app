#include <iostream>
#include <cstdio>

#include "fd_mosquitto.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

fd_mosquitto::config::config()
    : m_host("localhost")
{
}

void fd_mosquitto::config::parse(const std::string &file_name)
{
    FILE *file;
    rapidjson::Document document;
    char buffer[500];

    file = fopen(file_name.c_str(), "r");
    rapidjson::FileReadStream input_stream{file, buffer, sizeof(buffer)};
    document.ParseStream(input_stream);

    if (!document.HasMember("host")) {
        std::cout << "Failure, can not proceed without host" << std::endl;
        goto cleanup;
    }
    m_host = document["host"].GetString();

cleanup:
    fclose(file);
}

fd_mosquitto::fd_mosquitto(std::vector<std::string> &topics)
    : m_topics(topics)
    , m_config()
{
    mosqpp::lib_init();
    m_config.parse(CONFIG_FILE);
}

fd_mosquitto::~fd_mosquitto()
{
    baseclass::disconnect();
    mosqpp::lib_cleanup();
}

int fd_mosquitto::connect()
{
    return baseclass::connect(m_config.host().c_str());
}

void fd_mosquitto::on_connect(int rc)
{
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Connection failure" << std::endl;
        return;
    }

    for (auto topic: m_topics) {
        if (mosqpp::mosquittopp::subscribe(NULL, topic.c_str()) != MOSQ_ERR_SUCCESS)
            std::cout << "Failed to subscribe to topic: " << topic.c_str() << std::endl;
    }
}

void fd_mosquitto::on_subscribe(int mid, int qos_count, const int *qos_list)
{
    std::cout << "Subscribed" << std::endl;
}

void fd_mosquitto::on_message(const struct mosquitto_message *message)
{
    std::cout << "Received message for topic " << message->topic << std::endl;
}
