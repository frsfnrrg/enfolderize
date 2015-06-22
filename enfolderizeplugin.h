#ifndef ENFOLDERIZEPLUGIN_H
#define ENFOLDERIZEPLUGIN_H

#include <kabstractfileitemactionplugin.h>
#include <KUrl>


//#include <konq_dndpopupmenuplugin.h>


class KJob;
class KDialog;

/**
 * @brief Enfolderize action for Dolphin.
 *
 * Put selected stuff (folders, files) into a new folder.
 */
class EnfolderizePlugin_RMenu : public KAbstractFileItemActionPlugin {
    Q_OBJECT
  public:
    EnfolderizePlugin_RMenu(QObject *parent, const QList<QVariant> &args);
    virtual ~EnfolderizePlugin_RMenu();

    virtual QList<QAction *>
    actions(const KFileItemListProperties &fileItemInfos,
            QWidget *parentWidget);

private slots:
    void act();

  private:
    QWidget* mainWindow;
    QAction* action;
    KUrl::List itemsToMove;
};

class EnfolderizeOperation : public QObject {
    Q_OBJECT
public:
    EnfolderizeOperation(KUrl::List itemsToMove, QWidget* window);
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

#endif // ENFOLDERIZEPLUGIN_H
