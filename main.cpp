#include "mainwindow.h"

#include <QApplication>

///
/// \brief Это функция main, которая
/// \param argc Это количество аргументов
/// \param argv Это массив элементов
/// \return Возвращает результат выполнения программы
///
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
