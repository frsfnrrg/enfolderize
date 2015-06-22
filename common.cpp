#include "flags.h"
#include "common.h"

#include <KLineEdit>
#include <QVBoxLayout>
#include <KAction>

#include <KIO/CopyJob>
#include <KIO/Job>
#include <kio/fileundomanager.h>
#include <KIO/JobUiDelegate>

KAction *makeEnfolderizeAction(QObject *parent) {
#if DEBUG
    const QString actionName("Enfolderize (" __TIME__ ")");
#else
    const QString actionName("Place in New Folder");
#endif
    return new KAction(KIcon("draw-spiral"), actionName, parent);
}

KUrl parentOfUrls(KUrl::List x) {
    if (x.isEmpty()) {
        return KUrl();
    }
    // list is not empty
    QString scheme = x[0].scheme();
    QString authority = x[0].authority();

    QStringList dir = x[0].directory().split('/', QString::SkipEmptyParts);

    for (int i = 1; i < x.length(); i++) {
        const KUrl url = x.at(i);
        if (url.scheme() != scheme || url.authority() != authority) {
            return KUrl();
        }
        QStringList merge;
        const QStringList ud =
            url.directory().split('/', QString::SkipEmptyParts);

        QStringList::const_iterator uend = ud.constEnd();
        QStringList::const_iterator dend = dir.constEnd();
        for (QStringList::const_iterator u = ud.constBegin(),
                                         d = dir.constBegin();
             u != uend && d != dend; ++u, ++d) {
            if (*u == *d) {
                merge.append(*u);
            } else {
                break;
            }
        }
        dir = merge;
    }
    KUrl final;
    final.setPath("/" + dir.join("/"));
    final.setScheme(scheme);
    final.setAuthority(authority);
    return final;
}

EnfolderizeOperation::EnfolderizeOperation(KUrl::List items, QWidget *win,
                                           KUrl target) {
    itemsToMove = items;
    targetFolder = target;
    window = win;
    nameDialog = 0;
    folderName = QString();
}

void EnfolderizeOperation::gotName() {
    if (nameDialog) {
        disconnect(nameDialog, SIGNAL(destroyed(QObject *)), this,
                   SLOT(aborted()));
    }

    targetFolder.addPath(folderName);

    KIO::SimpleJob *job = KIO::mkdir(targetFolder);
    QObject::connect(job, SIGNAL(finished(KJob *)), this,
                     SLOT(mkdirComplete(KJob *)));
    KIO::FileUndoManager::self()->recordJob(KIO::FileUndoManager::Mkdir,
                                            KUrl::List(), targetFolder, job);
    job->setProperty("isMkdirJob",
                     true); // KDE5: cast to MkdirJob in slotResult instead
    if (window) {
        job->ui()->setWindow(window);
    }
    job->ui()->setAutoErrorHandlingEnabled(false);
}

void EnfolderizeOperation::aborted() { delete this; }

void EnfolderizeOperation::recordName(QString name) { folderName = name; }

void EnfolderizeOperation::queryFolderName() {
    // See kdelibs/kfile/knewfilemenu.cpp for model
    // TODO: eventually, apply i18n
    nameDialog = new KDialog(window);
    nameDialog->setModal(true);
    nameDialog->setAttribute(Qt::WA_DeleteOnClose);
    nameDialog->setButtons(KDialog::Ok | KDialog::Cancel);
    nameDialog->setCaption("New Folder for Items");

    KLineEdit *lineEdit = new KLineEdit();
    lineEdit->setClearButtonShown(true);
    lineEdit->setText("New Folder");
    connect(lineEdit, SIGNAL(textChanged(QString)), this,
            SLOT(recordName(QString)));
    recordName(lineEdit->text());

    QWidget *central = new QWidget(nameDialog);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(
        new QLabel(QString("Create new folder to hold %1 files in %2").arg(
            QString::number(itemsToMove.size()), targetFolder.path())));
    layout->addWidget(lineEdit);

    nameDialog->setMainWidget(central);
    connect(nameDialog, SIGNAL(accepted()), this, SLOT(gotName()));
    connect(nameDialog, SIGNAL(destroyed(QObject *)), this, SLOT(aborted()));

    nameDialog->show();
    lineEdit->selectAll();
    lineEdit->setFocus();
}

void EnfolderizeOperation::start() {
    if (targetFolder.isEmpty()) {
        targetFolder = parentOfUrls(itemsToMove);
        if (targetFolder.isEmpty()) {
            // i.e, common folder procedure failed
            delete this;
            return;
        }
    }

    // launch modal dialog to pick name
    queryFolderName();
}

void EnfolderizeOperation::mkdirComplete(KJob *oldjob) {
    if (oldjob->error()) {
        static_cast<KIO::Job *>(oldjob)->ui()->showErrorMessage();
        delete this;
        return;
    }

    KIO::CopyJob *copyjob =
        KIO::move(itemsToMove, targetFolder, KIO::Overwrite);
    KIO::FileUndoManager::self()->recordJob(KIO::FileUndoManager::Move,
                                            itemsToMove, targetFolder, copyjob);
    copyjob->setProperty("isCopyJob", true);
    if (window) {
        copyjob->ui()->setWindow(window);
    }
    QObject::connect(copyjob, SIGNAL(result(KJob *)), this,
                     SLOT(moveComplete(KJob *)));
}

void EnfolderizeOperation::moveComplete(KJob *oldjob) {
    if (oldjob->error()) {
        static_cast<KIO::Job *>(oldjob)->ui()->showErrorMessage();
    }

    delete this;
}
