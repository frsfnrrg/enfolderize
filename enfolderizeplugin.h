#pragma once

#define slots

#include <kabstractfileitemactionplugin.h>
#include <QUrl>

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

    QList<QAction *> actions(const KFileItemListProperties &fileItemInfos,
            QWidget *parentWidget) override;

private slots:
    void act();

  private:
    QWidget* mainWindow;
    QAction* action;
    QList<QUrl> itemsToMove;
};

