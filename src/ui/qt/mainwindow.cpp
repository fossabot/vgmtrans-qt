#include <qtextedit.h>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QtWidgets>
#include "mainwindow.h"
#include "QtVGMRoot.h"
#include "RawFileListView.h"
#include "VGMFileListView.h"
#include "VGMCollListView.h"
#include "MdiArea.h"
#include "Helpers.h"

const int defaultWindowWidth = 800;
const int defaultWindowHeight = 600;
const int defaultCollListHeight = 140;
const int defaultFileListWidth = 200;
const int splitterHandleWidth = 1;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  //  createTopbar();
  setUnifiedTitleAndToolBarOnMac(true);

  rawFileListView = new RawFileListView();
  vgmFileListView = new VGMFileListView();
  vgmCollListView = new VGMCollListView();

  vertSplitter = new QSplitter(Qt::Vertical, this);
  horzSplitter = new QSplitter(Qt::Horizontal, vertSplitter);
  vertSplitterLeft = new QSplitter(Qt::Vertical, horzSplitter);

  QList<int> sizes(
      {defaultWindowHeight - defaultCollListHeight, defaultCollListHeight});

  vertSplitter->addWidget(horzSplitter);
  vertSplitter->addWidget(vgmCollListView);
  vertSplitter->setStretchFactor(0, 1);
  vertSplitter->setSizes(sizes);
  vertSplitter->setHandleWidth(splitterHandleWidth);

  sizes = QList<int>(
      {defaultFileListWidth, defaultWindowWidth - defaultFileListWidth});

  horzSplitter->addWidget(vertSplitterLeft);
  horzSplitter->addWidget(MdiArea::getInstance());
  horzSplitter->setStretchFactor(1, 1);
  horzSplitter->setSizes(sizes);
  horzSplitter->setHandleWidth(splitterHandleWidth);
  horzSplitter->setMinimumSize(100, 100);
  horzSplitter->setMaximumSize(500, 0);
  horzSplitter->setCollapsible(0, false);
  horzSplitter->setCollapsible(1, false);

  vertSplitterLeft->addWidget(rawFileListView);
  vertSplitterLeft->addWidget(vgmFileListView);
  vertSplitterLeft->setHandleWidth(splitterHandleWidth);

  setCentralWidget(vertSplitter);
  resize(defaultWindowWidth, defaultWindowHeight);

  setAcceptDrops(true);
}

MainWindow::~MainWindow() {}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
  event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
  event->acceptProposedAction();
}

void MainWindow::openFile(QString path) {
  qtVGMRoot.OpenRawFile(qstringTowchar_t(path));
}

void MainWindow::dropEvent(QDropEvent *event) {
  const QMimeData *mimeData = event->mimeData();

  if (mimeData->hasUrls()) {
    QList<QUrl> urlList = mimeData->urls();
    for (int i = 0; i < urlList.size() && i < 32; ++i) {
      QString url = urlList.at(i).toLocalFile();
      openFile(url);
    }
  }

  setBackgroundRole(QPalette::Dark);
  event->acceptProposedAction();
}
