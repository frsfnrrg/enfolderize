#pragma once

#define slots

#include <KIO/DndPopupMenuPlugin>

#include <QList>
#include <QUrl>

class EnfolderizePlugin_DND : public KIO::DndPopupMenuPlugin {
    Q_OBJECT
  public:
    EnfolderizePlugin_DND(QObject *parent, const QList<QVariant> &args);
    virtual ~EnfolderizePlugin_DND();

    QList<QAction*> setup(const KFileItemListProperties& popupMenuInfo,
            const QUrl& destination) override;

private slots:
    void act();

  private:
    QAction* action;
    QUrl targetFolder;
    QList<QUrl> itemsToMove;
};
