#include "drugs.h"
#include "dlg_count.h"
#include <QMessageBox>

extern QVector<ItemInfo> g_ItemList;

drugs::drugs(RoleInfo *roleInfo, MapItem *bag_item)
: QWidget(NULL), myRole(roleInfo), m_bag_item(bag_item)
{
	ui.setupUi(this);

	
	AdjustTableWidget();
	DisplayItemList();

	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(cellClicked(int, int)));
}

drugs::~drugs()
{

}
void drugs::AdjustTableWidget(void)
{
	ui.tableWidget->setColumnHidden(0, true);

	ui.tableWidget->setColumnWidth(2, 50);
	ui.tableWidget->setColumnWidth(3, 100);
	ui.tableWidget->setColumnWidth(5, 50);

	ui.tableWidget->setColumnWidth(4, 250);	//��Ҫ����Ϊʹ��ʣ�����п��ȡ�
}

bool drugs::DisplayItemList(void)
{
	quint32 i = 0;
	quint32 n = g_ItemList.size();
	foreach(const ItemInfo &item, g_ItemList)
	{
		//����ʾԪ���������Ʒ������ʾ��ҩƷ
		if (item.gold > 0 || item.ID < 200000 || item.ID >= 201000)
		{
			continue;
		}
		ui.tableWidget->setRowCount(i + 1);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(item.ID)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(item.name));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(item.level)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(item.coin)));
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(item.EffectInfo));
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromLocal8Bit("����")));

		++i;
	}

	return true;
}

void drugs::cellClicked(int row, int column)
{
	//������Ϊ�����������ϵ����ˣ�����Ϊ����빺�����
	if (column == 5)
	{
		quint32 nCount, nCost;
		quint32 ID = ui.tableWidget->item(row, 0)->text().toUInt();		//��������ߵ�ID
		quint32 price = ui.tableWidget->item(row, 3)->text().toUInt();  //��������ߵĵ���
		quint32 nMaxCount = myRole->coin / price;						//��ҵ�ǰ�ʽ�ɹ�������������
		if (nMaxCount > 9999)
		{
			nMaxCount = 9999;											//���������������9999
		}

		//��������Ի���
		dlg_count *dlg = new dlg_count(this, QString::fromLocal8Bit("������"), nMaxCount);
		if (QDialog::Accepted == dlg->exec())
		{
			nCount = dlg->getCount();
			nCost = price * nCount;
			if (nCost > myRole->coin)
			{
				QString message = QString::fromLocal8Bit("���˲�Ҫ̫̰�ģ������е��ʽ����ֻ�ܹ���") + QString::number(nMaxCount);
				QMessageBox::critical(this, QString::fromLocal8Bit("����"), message);
			}
			else
			{
				myRole->coin -= nCost;
				m_bag_item->insert(ID, m_bag_item->value(ID) + nCount);
			}	
		}
	}
}