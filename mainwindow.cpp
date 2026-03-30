#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QStyle>
#include <QStatusBar>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Current working directory:" << QDir::currentPath();
    qDebug() << "Database file path:" << QDir::currentPath() + "/passwords.db";

    if (!dbManager.open("passwords.db")) {
        QMessageBox::critical(this, tr("Database Error"),
                              tr("Failed to open database: %1").arg(dbManager.database().lastError().text()));
        return;
    }
    if (!dbManager.initializeSchema()) {
        QMessageBox::critical(this, tr("Database Error"),
                              tr("Failed to initialize database schema."));
        return;
    }

    repository = new PasswordRepository(dbManager.database());
    model = new PasswordTableModel(this);
    loadData();

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::SelectedClicked
        );
    ui->tableView->horizontalHeader()->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::TitleColumn, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::IdColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::UsernameColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::PasswordColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::WebsiteColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::CategoryColumn, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(PasswordTableModel::UpdatedAtColumn, QHeaderView::ResizeToContents);

    connect(model, &PasswordTableModel::dataChanged, this, [this](const QModelIndex &topLeft, const QModelIndex &, const QVector<int> &) {
        int row = topLeft.row();
        PasswordEntry e = model->entryAt(row);
        if (!repository->update(e)) {
            qDebug() << "Failed to update entry in DB";
        } else {
            statusBar()->showMessage(tr("Changes saved"), 2000);
        }
    });

    ui->actionNew->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    ui->actionEdit->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    ui->actionDelete->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));
    ui->actionSave->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->actionExit->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNew);
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onEdit);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDelete);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    updateEmptyState();
    statusBar()->showMessage(tr("Ready"), 2000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete repository;
}

void MainWindow::loadData()
{
    QList<PasswordEntry> entries = repository->loadAll();
    model->setEntries(entries);
    updateEmptyState();
    statusBar()->showMessage(tr("Loaded %1 records").arg(entries.size()), 2000);
}

void MainWindow::applySearchFilter(const QString &text)
{
    if (text.isEmpty()) {
        ui->tableView->setModel(model);
    } else {
        QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
        proxy->setSourceModel(model);
        proxy->setFilterKeyColumn(PasswordTableModel::TitleColumn);
        proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
        proxy->setFilterFixedString(text);
        ui->tableView->setModel(proxy);
    }
    updateEmptyState();
}

void MainWindow::updateEmptyState()
{
    bool empty = (model->rowCount() == 0);
    ui->emptyLabel->setVisible(empty);
    ui->tableView->setVisible(!empty);
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    applySearchFilter(text);
}

void MainWindow::onNew()
{
    PasswordEntry empty;
    empty.title = "";
    empty.username = "";
    empty.password = "";
    empty.website = "";
    empty.category = "";
    empty.updatedAt = QDateTime::currentDateTime();
    if (!repository->insert(empty)) {
        QMessageBox::warning(this, tr("Error"), tr("Could not create new record."));
        return;
    }
    loadData();
    int newRow = model->rowCount() - 1;
    QModelIndex idx = model->index(newRow, PasswordTableModel::TitleColumn);
    ui->tableView->scrollTo(idx);
    ui->tableView->setCurrentIndex(idx);
    ui->tableView->edit(idx);
    statusBar()->showMessage(tr("New record added"), 2000);
}

void MainWindow::onEdit()
{
    QModelIndex current = ui->tableView->currentIndex();
    if (!current.isValid()) return;
    ui->tableView->edit(current);
    statusBar()->showMessage(tr("Editing..."), 2000);
}

void MainWindow::onDelete()
{
    QModelIndex current = ui->tableView->currentIndex();
    if (!current.isValid()) return;
    int row = current.row();
    PasswordEntry e = model->entryAt(row);
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Delete Record"),
        tr("Are you sure you want to delete the selected record?"),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (!repository->remove(e.id)) {
            QMessageBox::warning(this, tr("Error"), tr("Could not delete record."));
            return;
        }
        loadData();
        statusBar()->showMessage(tr("Record deleted"), 2000);
    }
}

void MainWindow::onSave()
{
    statusBar()->showMessage(tr("All changes already saved automatically."), 2000);
}

void MainWindow::onExit()
{
    close();
}
