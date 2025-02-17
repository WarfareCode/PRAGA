#include "dialogPragaProject.h"
#include "utilities.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QDialogButtonBox>


DialogPragaProject::DialogPragaProject(PragaProject *myProject) : DialogProject(myProject)
{
    project_ = myProject;
}

void DialogPragaProject::accept()
{
    project_->createPragaProject(lineEditProjectPath->text(), lineEditProjectName->text(), lineEditProjectDescription->text());

    QDialog::done(QDialog::Accepted);
}


#ifdef NETCDF
    bool chooseNetCDFVariable(NetCDFHandler* netCDF, int* varId, QDateTime* firstDateTime, QDateTime* lastDateTime)
    {
        // check
        if (! netCDF->isLoaded())
        {
            QMessageBox::information(nullptr, "No data", "Load NetCDF before");
            return false;
        }
        if (! netCDF->isTimeReadable())
        {
            QMessageBox::information(nullptr, "Wrong time", "Needs POSIX time (seconds since 1970-01-01)");
            return false;
        }

        QDialog myDialog;
        QVBoxLayout mainLayout;
        QVBoxLayout layoutVariable;
        QHBoxLayout layoutDate;
        QHBoxLayout layoutOk;
        QDateTime firstTime, lastTime;

        myDialog.setWindowTitle("Export NetCDF data series");

        // Variables
        QLabel *VariableLabel = new QLabel("<b>Variable:</b>");
        layoutVariable.addWidget(VariableLabel);

        unsigned int nrVariables = netCDF->getNrVariables();
        std::vector<QRadioButton*> buttonVars;

        for (unsigned int i = 0; i < nrVariables; i++)
        {
            QString varName = QString::fromStdString(netCDF->variables[i].getVarName());
            buttonVars.push_back(new QRadioButton(varName));

            layoutVariable.addWidget(buttonVars[i]);
        }

        // void space
        layoutVariable.addWidget(new QLabel());

        // date widgets
        Crit3DTime t0, t1;
        t0 = netCDF->getFirstTime();
        t1 = netCDF->getLastTime();
        firstTime = getQDateTime(t0);
        lastTime = getQDateTime(t1);
        QString str1, str2;
        str1 = firstTime.toString();
        str2 = lastTime.toString();

        QDateTimeEdit *firstDateEdit = new QDateTimeEdit;
        firstDateEdit->setDateTimeRange(firstTime, lastTime);
        firstDateEdit->setDateTime(firstTime);

        QLabel *firstDateLabel = new QLabel("<b>First Date:</b>");
        firstDateLabel->setBuddy(firstDateEdit);

        QDateTimeEdit *lastDateEdit = new QDateTimeEdit;
        lastDateEdit->setDateTimeRange(firstTime, lastTime);
        lastDateEdit->setDateTime(lastTime);

        QLabel *lastDateLabel = new QLabel("<b>Last Date:</b>");
        lastDateLabel->setBuddy(lastDateEdit);

        layoutDate.addWidget(firstDateLabel);
        layoutDate.addWidget(firstDateEdit);

        layoutDate.addWidget(lastDateLabel);
        layoutDate.addWidget(lastDateEdit);

        //void space
        layoutDate.addWidget(new QLabel());

        //Ok button
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        myDialog.connect(&buttonBox, SIGNAL(accepted()), &myDialog, SLOT(accept()));
        myDialog.connect(&buttonBox, SIGNAL(rejected()), &myDialog, SLOT(reject()));
        layoutOk.addWidget(&buttonBox);

        // Main layout
        mainLayout.addLayout(&layoutVariable);
        mainLayout.addLayout(&layoutDate);
        mainLayout.addLayout(&layoutOk);

        myDialog.setLayout(&mainLayout);
        myDialog.exec();

        if (myDialog.result() != QDialog::Accepted)
            return false;

        // assing values
        *firstDateTime = firstDateEdit->dateTime();
        *lastDateTime = lastDateEdit->dateTime();

        bool isVarSelected = false;
        unsigned int i = 0;
        while (i < nrVariables && ! isVarSelected)
        {
            if (buttonVars[i]->isChecked())
            {
                *varId = netCDF->variables[i].id;
                isVarSelected = true;
            }
            i++;
        }

        return isVarSelected;
    }

#endif

