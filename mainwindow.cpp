#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVideoWidget>
#include <QDebug>
#include <QPushButton>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QStandardPaths>

///
/// \brief Это конструктор главного окна
/// \param parent Принимает ссылку на виджет-родитель
///
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
	QFile data("positions.txt");
	if (data.open(QIODevice::ReadOnly)) {
		qDebug() << "Open file.";
		while (!data.atEnd()) {
			QString rawtext = QString::fromLocal8Bit(data.readLine());
			rawtext.chop(1);
			qint64 value = rawtext.section("|", -1).toLongLong();
			QString key = rawtext.section("|", -2, -2);
			positions->insert(key, value);
		}
	} else {
		qDebug() << "Couldn't open file.";
	}
    QAction *plylstActAdd = new QAction("Add media");
    ui->menubar->addAction(plylstActAdd);
    connect(plylstActAdd, &QAction::triggered, this, &MainWindow::editPlaylistAdd);

    ui->gridLayout->setContentsMargins(10, 10, 10, 10);
    ui->centralwidget->setLayout(ui->gridLayout);
	model->setStringList(*plylstDouble);
	scroll->setModel(model);

    playlist->setCurrentIndex(0);

    video = new QVideoWidget(this);
    ui->horizontalLayout->addWidget(scroll, 1);
    ui->horizontalLayout->addWidget(video, 2);
    player->setVideoOutput(video);
    player->setMedia(playlist->currentMedia());

	QHBoxLayout *ctrlsBox = new QHBoxLayout;
	ctrlsBox->addWidget(volume, 1);
	ctrlsBox->addWidget(time, 3);
	ctrlsBox->addWidget(watch, 1);
	ui->gridLayout->addLayout(ctrlsBox, 2, 0);
	volume->setRange (0, 100);
	volume->setValue(player->volume());
	connect(volume, &QSlider::sliderMoved, player, &QMediaPlayer::setVolume);
	connect(time, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    QHBoxLayout *box = new QHBoxLayout;
    ui->gridLayout->addLayout(box, 3, 0);
    QPushButton *stop = new QPushButton(QIcon(":/icons/stop.png"), "", this);
    QPushButton *start = new QPushButton(QIcon(":/icons/start.png"), "", this);
	box->addWidget(stop);
	box->addWidget(start);
	connect(stop, &QPushButton::pressed, player, &QMediaPlayer::pause);
	connect(start, &QPushButton::pressed, player, &QMediaPlayer::play);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handleStateChange);
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setTimeVal(qint64)));

    connect(scroll, &QListView::doubleClicked, this, &MainWindow::chooseMedia);
    connect(scroll, &GoodListView::rightClicked, this, &MainWindow::deleteMedia);

}

///
/// \brief Это деструктор главного окна
///
MainWindow::~MainWindow() {
    delete ui;
}

///
/// \brief Этот слот устанавливает последнюю сохранённую позицию видеозаписи
///
void MainWindow::setDuration() {
	time->setMaximum(player->duration());
}

///
/// \brief Этот слот выводит, сколько времени прошло с момента начала воспроизведения и общую длительность контента
/// \param timeVal Это позиция медиаконтента на даннный момент
///
void MainWindow::setTimeVal(qint64 timeVal) {
    time->setValue(timeVal);
    int secsD = player->duration() / 1000;
    int milisecsD = player->duration() % 100;
    int secs = timeVal /1000;
    int milisecs = timeVal % 100;
    QString currTime = QString::number(secs) + ":" + QString::number(milisecsD) + "/" + QString::number(secsD) + ":" + QString::number(milisecs);
    watch->setText(currTime);
}

///
/// \brief Этот слот устанавливает длительность видеозаписи, когда контент полностью загружен и готов к воспроизведению
/// \param newState Принимает состояние, в которое перешёл плеер
///
void MainWindow::handleStateChange(QMediaPlayer::MediaStatus newState) {
    if (newState == QMediaPlayer::LoadedMedia) {
		setDuration();
    }
}

///
/// \brief Это слот добавления контента в плейлист
///
void MainWindow::editPlaylistAdd() {
    QUrl filepath = QFileDialog::getOpenFileUrl(this, "Add media", QUrl(""), "Videos (*.mp4 *.avi *.mov *.mkv)");
    plylstDouble->append(filepath.toLocalFile());
    playlist->addMedia(filepath);
    qDebug() << plylstDouble->back();
    qDebug() << playlist->media(plylstDouble->size() - 1).canonicalUrl();  // Это соотношение двух списков, смотреть для референса
    model->setStringList(*plylstDouble);

}

///
/// \brief Это слот, который устанавливает выбранный контент в видеоплеер
/// \param index Принимает индекс выбранного медиаконтента
///
void MainWindow::chooseMedia(QModelIndex index) {
	positions->insert(player->currentMedia().canonicalUrl().toString(), player->position());
    player->setMedia(playlist->media(index.row()));
	if (positions->contains(playlist->media(index.row()).canonicalUrl().toString())) {
		player->setPosition(positions->value(playlist->media(index.row()).canonicalUrl().toString()));
	}
}

///
/// \brief Это слот удаления контента из плейлиста
/// \param index Принимает индекс контента, который нужно удалить
///
void MainWindow::deleteMedia(QModelIndex index) {
    qDebug() << "You chose to delete media #" + QString::number(index.row());
    QMessageBox *box = new QMessageBox(this);
    box->setText("Do you want to delete this video?");
    box->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int result = box->exec();
    if (result == 16384) {
        playlist->setCurrentIndex(index.row());
        player->setMedia(playlist->currentMedia());
        playlist->removeMedia(index.row());
        plylstDouble->removeAt(index.row());
        model->setStringList(*plylstDouble);
    }
}

///
/// \brief Это перегрузка эвента закрытия окна приложения
/// \param event Принимает ссылку на эвент закрытия окна
///
void MainWindow::closeEvent(QCloseEvent *event) {
	positions->insert(player->currentMedia().canonicalUrl().toString(), player->position());

	QFile data("positions.txt");
	if (data.open(QIODevice::WriteOnly)) {
		qDebug() << "File open";
		for (QMap<QString, qint64>::iterator i = positions->begin(); i != positions->end(); i++) {
			if (i.key() != "") {
				QString text = i.key() + "|" + QString::number(i.value()) + "\n";
				char *kek = text.toLocal8Bit().data();
				data.write(kek);
			}
		}
		data.close();
	};
	event->accept();
}



