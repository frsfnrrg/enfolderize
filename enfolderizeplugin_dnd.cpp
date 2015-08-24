#include "enfolderizeplugin_dnd.h"
#include "common.h"
#include "flags.h"

#include <QAction>

#include <KFileItemListProperties>
#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(EnfolderizePluginDNDFactory,
                      "enfolderizeplugin_dnd.json",
                    registerPlugin<EnfolderizePlugin_DND>();)

EnfolderizePlugin_DND::EnfolderizePlugin_DND(QObject *parent,
                                             const QList<QVariant> &args)
    : KIO::DndPopupMenuPlugin(parent) {
    Q_UNUSED(args);
    action = 0;
}

EnfolderizePlugin_DND::~EnfolderizePlugin_DND() {
    if (action) {
        delete action;
    }
}

QList<QAction *> EnfolderizePlugin_DND::setup(const KFileItemListProperties &popupMenuInfo,
                                  const QUrl& destination) {

    itemsToMove = popupMenuInfo.urlList();
    targetFolder = destination;

    // only enfolderize multiple items
    if (itemsToMove.size() < MIN_NUMBER_OF_OBJECTS) {
        return QList<QAction* >();
    }

    if (!action) {
        action = makeEnfolderizeAction(this);
        connect(action, SIGNAL(triggered(bool)), SLOT(act()));
    }
    return  QList<QAction *>() << action;
}

void EnfolderizePlugin_DND::act() {
    // The operation deletes itself on completion (or failure)
    EnfolderizeOperation *op =
        new EnfolderizeOperation(itemsToMove, 0, targetFolder);
    op->start();
}

#include "enfolderizeplugin_dnd.moc"
