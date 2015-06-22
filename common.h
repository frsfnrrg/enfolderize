#pragma once

#include <KUrl>

class KAction;
class KDialog;
class KJob;

KAction* makeEnfolderizeAction(QObject* parent);

class EnfolderizeOperation : public QObject {
    Q_OBJECT
public:
    EnfolderizeOperation(KUrl::List itemsToMove, QWidget* window=0, KUrl targetFolder=KUrl());
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
    KUrl targetFolder;
    KUrl::List itemsToMove;
    QWidget* window;
    KDialog *nameDialog;
};
