#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QPersistentModelIndex>
#include <QMap>
#include <QCloseEvent>
#include "goodlistview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///
/// \brief Это класс главного окна приложения
///
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
	///
	/// \brief Это конструктор главного окна
	/// \param parent Принимает ссылку на виджет-родитель
	///
    MainWindow(QWidget *parent = nullptr);

	///
	/// \brief Это деструктор главного окна
	///
    ~MainWindow();

	///
	/// \brief Это объект медиаплеер, который воспроизводит медиафайлы
	///
    QMediaPlayer *player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);

	///
	/// \brief Это объект плейлист, который хранит в себе медиаконтент
	///
    QMediaPlaylist *playlist = new QMediaPlaylist(player);

	///
	/// \brief Этот виджет отвечает за показ контента
	///
    QVideoWidget *video;

	///
	/// \brief Этот слайдер отвечает за громкость
	///
	QSlider *volume = new QSlider(Qt::Horizontal);

	///
	/// \brief Этот слайдер отвечает за позицию медиаконтента
	///
    QSlider *time = new QSlider(Qt::Horizontal);

	///
	/// \brief Этот лейбл показывает длительность
	///
    QLabel *watch = new QLabel("00:00/00:00");

	///
	/// \brief Этот плейлист - дублёр основного, он нужен для отображения плейлиста в ListView
	///
    QStringList *plylstDouble = new QStringList();

	///
	/// \brief Это - модель дублёра плейлиста для его отображения
	///
    QStringListModel *model = new QStringListModel();

	///
	/// \brief Это - тот самый ListView для отображения
	///
    GoodListView *scroll = new GoodListView();

	///
	/// \brief Это ассоциативный массив для хранения считанных сохранённых позиций медиаконтента
	///
	QMap<QString, qint64> *positions = new QMap<QString, qint64>;

	///
	/// \brief Это перегрузка эвента закрытия окна приложения
	/// \param event Принимает ссылку на эвент закрытия окна
	///
	void closeEvent(QCloseEvent *event);


public slots:
	///
	/// \brief Этот слот устанавливает последнюю сохранённую позицию видеозаписи
	///
    void setDuration();

	///
	/// \brief Этот слот выводит, сколько времени прошло с момента начала воспроизведения и общую длительность контента
	/// \param timeVal Это позиция медиаконтента на даннный момент
	///
    void setTimeVal(qint64 time);

	///
	/// \brief Этот слот устанавливает длительность видеозаписи, когда контент полностью загружен и готов к воспроизведению
	/// \param newState Принимает состояние, в которое перешёл плеер
	///
    void handleStateChange(QMediaPlayer::MediaStatus newState);

	///
	/// \brief Это слот добавления контента в плейлист
	///
    void editPlaylistAdd();

	///
	/// \brief Это слот, который устанавливает выбранный контент в видеоплеер
	/// \param index Принимает индекс выбранного медиаконтента
	///
    void chooseMedia(QModelIndex index);

	///
	/// \brief Это слот удаления контента из плейлиста
	/// \param index Принимает индекс контента, который нужно удалить
	///
    void deleteMedia(QModelIndex index);
private:
	///
	/// \brief Этот объект содержит интерфейс программы
	///
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
