#include "enfolderizeplugin.h"
#include "common.h"
#include "flags.h"

#include <QAction>
#include <KFileItemListProperties>
#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(EnfolderizePluginFactory, "enfolderizeplugin.json", registerPlugin<EnfolderizePlugin_RMenu>();)

EnfolderizePlugin_RMenu::EnfolderizePlugin_RMenu(QObject *parent,
                                                 const QList<QVariant> &args)
    : KAbstractFileItemActionPlugin(parent) {
    Q_UNUSED(args);
    action = 0;
}

EnfolderizePlugin_RMenu::~EnfolderizePlugin_RMenu() {
    if (action) {
        delete action;
    }
}

QList<QAction *>
EnfolderizePlugin_RMenu::actions(const KFileItemListProperties &fileItemInfos,
                                 QWidget *parentWidget) {
    // The parent is the main window: see dolphin/dolphincontextmenu.cpp
    mainWindow = parentWidget;
    itemsToMove = fileItemInfos.urlList();

    // Only enfolderize multiple items
    if (itemsToMove.size() < MIN_NUMBER_OF_OBJECTS) {
        return QList<QAction *>();
    }

    if (!action) {
        action = makeEnfolderizeAction(mainWindow);
        connect(action, SIGNAL(triggered(bool)), SLOT(act()));
    }

    return QList<QAction *>() << action;
}

void EnfolderizePlugin_RMenu::act() {
    // The operation deletes itself on completion (or failure)
    EnfolderizeOperation *op =
        new EnfolderizeOperation(itemsToMove, mainWindow);
    op->start();
}

#include "enfolderizeplugin.moc"

