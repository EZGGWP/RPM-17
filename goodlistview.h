#ifndef GOODLISTVIEW_H
#define GOODLISTVIEW_H
#include <QListView>
#include <QMouseEvent>
#include <QPersistentModelIndex>

///
/// \brief Это класс GoodListView, который является улучшенной версией QListView
///
class GoodListView : public QListView {
    Q_OBJECT
public:
	///
	/// \brief Это конструктор класса GoodListView - дополненного QListView
	///
    GoodListView();

	///
	/// \brief Это перегрузка эвента клика по элементам GoodListView
	/// \param event Принимает ссылку на эвент действия мыши
	///
    void mousePressEvent(QMouseEvent *event);

	///
	/// \brief Это перегрузка эвента двойного клика по элементам GoodListView
	/// \param event Принимает ссылку на эвент действия мыши
	///
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
	///
	/// \brief Это сигнал, который эмитится при нажатии по элементам правой кнопкой мыши
	/// \param index Передаёт индекс элемента, по которому было произведено нажатие
	///
    void rightClicked(QPersistentModelIndex index);
};

#endif // GOODLISTVIEW_H
