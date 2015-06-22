#pragma once

#include <kabstractfileitemactionplugin.h>
#include <KUrl>

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

