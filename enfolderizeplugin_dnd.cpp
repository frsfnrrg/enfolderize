#include "enfolderizeplugin_dnd.h"
#include "common.h"
#include "flags.h"

#include <KFileItemListProperties>
#include <KAction>

#include <KPluginFactory>
#include <KPluginLoader>

K_PLUGIN_FACTORY(EnfolderizePluginDNDFactory,
                 registerPlugin<EnfolderizePlugin_DND>();)
K_EXPORT_PLUGIN(EnfolderizePluginDNDFactory("enfolderizeplugin_dnd"))

EnfolderizePlugin_DND::EnfolderizePlugin_DND(QObject *parent,
                                             const QList<QVariant> &args)
    : KonqDndPopupMenuPlugin(parent) {
    Q_UNUSED(args);
    action = 0;
}

EnfolderizePlugin_DND::~EnfolderizePlugin_DND() {
    if (action) {
        delete action;
    }
}

void EnfolderizePlugin_DND::setup(const KFileItemListProperties &popupMenuInfo,
                                  KUrl destination,
                                  QList<QAction *> &pluginActions) {
    itemsToMove = popupMenuInfo.urlList();
    targetFolder = destination;

    // only enfolderize multiple items
    if (itemsToMove.size() < MIN_NUMBER_OF_OBJECTS) {
        return;
    }

    if (!action) {
        action = makeEnfolderizeAction(this);
        connect(action, SIGNAL(triggered(bool)), SLOT(act()));
    }
    pluginActions.append(action);
}

void EnfolderizePlugin_DND::act() {
    // The operation deletes itself on completion (or failure)
    EnfolderizeOperation *op =
        new EnfolderizeOperation(itemsToMove, 0, targetFolder);
    op->start();
}
