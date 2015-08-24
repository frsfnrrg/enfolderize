#include "flags.h"
#include "common.h"

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include <KIO/CopyJob>
#include <KIO/JobUiDelegate>
#include <KIO/Job>
#include <KIO/FileUndoManager>

#include <KGuiItem>
#include <KJobWidgets>

QAction *makeEnfolderizeAction(QObject *parent) {
    return new QAction(QIcon::fromTheme(QStringLiteral("draw-spiral")),
#if DEBUG
                       QStringLiteral actionName("Enfolderize (" __TIME__ ")")
#else
                       QStringLiteral("Place in New Folder"),
#endif
                       parent);
}

QUrl directoryOf(const QUrl& u) {
    QUrl u2 = u.adjusted(QUrl::StripTrailingSlash);
    return u2.adjusted(QUrl::RemoveFilename);
}

QUrl appendToFolder(const QUrl& u, const QString& s) {
    QUrl u2 = u;
    u2.setPath(u.path(QUrl::EncodeReserved).append(QChar::fromLatin1('/')) + s);
    return u2;
}

QUrl parentOfUrls(QList<QUrl> x) {
    if (x.isEmpty()) {
        return QUrl();
    }
    // list is not empty
    QString scheme = x[0].scheme();
    QString authority = x[0].authority();


    QStringList dir = directoryOf(x[0]).path().split(QChar::fromLatin1('/'), QString::SkipEmptyParts);

    for (int i = 1; i < x.length(); i++) {
        const QUrl url = x.at(i);
        if (url.scheme() != scheme || url.authority() != authority) {
            return QUrl();
        }
        QStringList merge;
        const QStringList ud =
            directoryOf(url).path().split(QChar::fromLatin1('/'), QString::SkipEmptyParts);

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
    QUrl final;
    final.setPath(dir.join(QChar::fromLatin1('/')).prepend(QChar::fromLatin1('/')));
    final.setScheme(scheme);
    final.setAuthority(authority);
    return final;
}

EnfolderizeOperation::EnfolderizeOperation(QList<QUrl> items, QWidget *win,
                                           QUrl target) {
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


    QUrl intermediateFolder = appendToFolder(targetFolder, folderName);
#if DEBUG
    qDebug("%s + %s -> %s", targetFolder.toString().toUtf8().constData(),
           folderName.toUtf8().constData(),
           intermediateFolder.toString().toUtf8().constData());
#endif
    targetFolder = intermediateFolder;

    KIO::SimpleJob *job = KIO::mkdir(targetFolder);
    QObject::connect(job, SIGNAL(finished(KJob *)), this,
                     SLOT(mkdirComplete(KJob *)));
    KIO::FileUndoManager::self()->recordJob(KIO::FileUndoManager::Mkdir,
                                            QList<QUrl>(), targetFolder, job);
    job->setProperty("isMkdirJob",
                     true);
    if (window) {
        KJobWidgets::setWindow(job, window);
    }
    job->ui()->setAutoErrorHandlingEnabled(false);
}

void EnfolderizeOperation::aborted() { delete this; }

void EnfolderizeOperation::recordName(QString name) { folderName = name; }

void EnfolderizeOperation::queryFolderName() {
    // TODO: eventually, apply i18n
    nameDialog = new QDialog(window);
    nameDialog->setModal(true);
    nameDialog->setAttribute(Qt::WA_DeleteOnClose);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    QPushButton* m_okButton = buttonBox->button(QDialogButtonBox::Ok);
    m_okButton->setDefault(true);
    m_okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(m_okButton, SIGNAL(clicked(bool)), nameDialog, SLOT(accept()));
    connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked(bool)), nameDialog, SLOT(reject()));

    nameDialog->setWindowTitle(QStringLiteral("New Folder for Items"));

    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setClearButtonEnabled(true);
    lineEdit->setText(QStringLiteral("New Folder"));
    connect(lineEdit, SIGNAL(textChanged(QString)), this,
            SLOT(recordName(QString)));
    recordName(lineEdit->text());

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(
        new QLabel(QStringLiteral("Create new folder to hold %1 files in %2").arg(
            QString::number(itemsToMove.size()), targetFolder.path())));
    layout->addWidget(lineEdit);
    layout->addWidget(buttonBox);

    nameDialog->setLayout(layout);
    connect(nameDialog, SIGNAL(accepted()), this, SLOT(gotName()));
    connect(nameDialog, SIGNAL(destroyed(QObject *)), this, SLOT(aborted()));

    nameDialog->show();
    lineEdit->selectAll();
    lineEdit->setFocus();
}

void EnfolderizeOperation::start() {
    if (targetFolder.isEmpty()) {
        targetFolder = parentOfUrls(itemsToMove);
#if DEBUG
        qDebug("%s : %s %s", targetFolder.toString().toUtf8().constData(),
               itemsToMove[0].toString().toUtf8().constData(),
               itemsToMove[1].toString().toUtf8().constData());
#endif
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
        KJobWidgets::setWindow(copyjob, window);
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
