#pragma once

#define slots

#include <QtCore/QUrl>
#include <QtCore/QObject>

class QAction;
class QDialog;
class KJob;

QAction* makeEnfolderizeAction(QObject* parent);

class EnfolderizeOperation : public QObject {
    Q_OBJECT
public:
    EnfolderizeOperation(QList<QUrl> itemsToMove, QWidget* window=0, QUrl targetFolder=QUrl());
    void start();
private slots:
    void mkdirComplete(KJob * job);
    void moveComplete(KJob * job);
    void gotName();
    void aborted();
    void recordName(QString name);
private:
    void queryFolderName();

    QString folderName;
    QUrl targetFolder;
    QList<QUrl> itemsToMove;
    QWidget* window;
    QDialog *nameDialog;
};
