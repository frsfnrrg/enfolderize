#pragma once

#include <konq_dndpopupmenuplugin.h>
#include <KUrl>

class EnfolderizePlugin_DND : public KonqDndPopupMenuPlugin {
    Q_OBJECT
  public:
    EnfolderizePlugin_DND(QObject *parent, const QList<QVariant> &args);
    virtual ~EnfolderizePlugin_DND();

    void setup(const KFileItemListProperties& popupMenuInfo,
            KUrl destination,
            QList<QAction*>& pluginActions);

private slots:
    void act();

  private:
    QAction* action;
    KUrl targetFolder;
    KUrl::List itemsToMove;
};
