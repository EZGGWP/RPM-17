#include "goodlistview.h"
#include <QDebug>

///
/// \brief Это конструктор класса GoodListView - дополненного QListView
///
GoodListView::GoodListView() : QListView() {

}

///
/// \brief Это перегрузка эвента клика по элементам GoodListView
/// \param event Принимает ссылку на эвент действия мыши
///
void GoodListView::mousePressEvent(QMouseEvent *event) {
    QPersistentModelIndex persIndex = indexAt(event->pos());
    QModelIndex index = QModelIndex(persIndex);
    if (event->button() == Qt::RightButton) {
        emit rightClicked(index);
    } else if (event->button() == Qt::LeftButton) {
        emit QListView::clicked(index);
    }
}

///
/// \brief Это перегрузка эвента двойного клика по элементам GoodListView
/// \param event Принимает ссылку на эвент действия мыши
///
void GoodListView::mouseDoubleClickEvent(QMouseEvent *event) {
    QPersistentModelIndex persIndex = indexAt(event->pos());
    QModelIndex index = QModelIndex(persIndex);
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(index);
    }
}

