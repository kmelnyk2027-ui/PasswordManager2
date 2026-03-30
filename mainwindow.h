#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "passwordmodel.h"
#include "databasemanager.h"
#include "passwordrepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNew();
    void onEdit();
    void onDelete();
    void onSave();
    void onExit();
    void onSearchTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    PasswordTableModel *model;
    DatabaseManager dbManager;
    PasswordRepository *repository;
    void loadData();
    void applySearchFilter(const QString &text);
    void updateEmptyState();
};

#endif
