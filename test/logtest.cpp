#include "spdlog/spdlog.h"
#include<iostream>
#include<memory>

void async_example();
void syslog_example();
void user_defined_example();
void err_handler_example();
namespace spd = spdlog;

int main(int,char*[]){
    try{
        auto console = spd::stdout_color_mt("console");
    }
}