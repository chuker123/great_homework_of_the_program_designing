#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QFontDatabase>
#include <QCoreApplication>


int main(int argc, char *argv[]) {
    // 在 main() 函数开头添加：
    qDebug() << "=== 资源文件检查 ===";
    qDebug() << "guanyu.png exists:" << QFile::exists(":/resources/characters/guanyu.png");
    qDebug() << "battle_bg_1.png exists:" << QFile::exists(":/resources/backgrounds/battle_bg_1.png");
    qDebug() << "card_attack.png exists:" << QFile::exists(":/resources/cards/card_attack.png");
    QApplication a(argc, argv);
    a.setApplicationName("千里走单骑");
    a.setApplicationDisplayName("千里走单骑");

    QString fontPath1 = QCoreApplication::applicationDirPath() + "/wind-xingkai.ttf";
    int fontId1 = QFontDatabase::addApplicationFont(fontPath1);
    if (fontId1 != -1) {
        QStringList fontFamilies1 = QFontDatabase::applicationFontFamilies(fontId1);
        if (!fontFamilies1.isEmpty()) {
            QString fontFamily1 = fontFamilies1.first();
            qDebug() << "字体加载成功（kai.ttf）：" << fontFamily1;
        }
    } else {
        qDebug() << "字体加载失败，请检查路径：" << fontPath1;
    }

    // 加载第二个字体（行楷）
    QString fontPath2 = QCoreApplication::applicationDirPath() + "/martial-xingkai.ttf";
    int fontId2 = QFontDatabase::addApplicationFont(fontPath2);
    if (fontId2 != -1) {
        QStringList fontFamilies2 = QFontDatabase::applicationFontFamilies(fontId2);
        if (!fontFamilies2.isEmpty()) {
            QString fontFamily2 = fontFamilies2.first();
            qDebug() << "字体加载成功（wind-xingkai.ttf）：" << fontFamily2;
        }
    } else {
        qDebug() << "字体加载失败，请检查路径：" << fontPath2;
    }


    QString fontPath3 = QCoreApplication::applicationDirPath() + "/song-kai.ttf";
    int fontId3 = QFontDatabase::addApplicationFont(fontPath3);
    if (fontId3 != -1) {
        QStringList fontFamilies3 = QFontDatabase::applicationFontFamilies(fontId3);
        if (!fontFamilies3.isEmpty()) {
            QString fontFamily3 = fontFamilies3.first();
            qDebug() << "字体加载成功（wind-xingkai.ttf）：" << fontFamily3;
        }
    } else {
        qDebug() << "字体加载失败，请检查路径：" << fontPath3;
    }


    MainWindow w;
    w.show();

    return a.exec();
}
