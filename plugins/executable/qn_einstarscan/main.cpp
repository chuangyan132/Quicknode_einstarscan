/*!
* @brief   
* @project qn_einstarscan
* @file    main.cpp
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-24 11:03
*/

#include <locale>
#include <string>
#include <iostream>
#include <qn_plugin.h>
#include <thread>
#include <QThread>

#include "../include/pluginbussiness.h"
//#include "./include/log.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <QApplication>


int main(int argc, char *argv[]) {
    // 设置内存泄露检测机制
#ifdef _WINDOWS_
    int CurrentFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    CurrentFlag |= _CRTDBG_LEAK_CHECK_DF;
    CurrentFlag &= ~_CRTDBG_CHECK_CRT_DF;
    _CrtSetDbgFlag(CurrentFlag);
#endif
	QApplication app(argc, argv);
	//app.setQuitOnLastWindowClosed(false);
	//QThread::sleep(10);

    //本地化支持中文
    std::locale loc = std::locale::global(std::locale(""));

    std::unique_ptr<PluginBussiness> handler = std::make_unique<PluginBussiness>();

    // set log callback in here, you can dump all logs of plugincore inner
    qn_set_log_callback(PluginBussiness::onHandleLog, handler.get());

    // 初始化
    handler->init(argc, argv);

    // 注册回调
    qn_set_message_callback(PluginBussiness::onHandleMessage, handler.get());

	app.exec();


    while (true) {
        std::string str;
        std::cin >> str;
        if (str.compare("exit") == 0) {
            break;
        } else {
            std::cout << "error command" << std::endl;
        }
    }

    // 结束
    handler->fini();

    // 还原locale
    std::locale::global(loc);

    //std::cin.get();

#ifdef _WINDOWS_
    system("PAUSE");
#endif

    return 0;
}
#include "main.moc"

