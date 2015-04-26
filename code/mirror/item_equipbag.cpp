#include "item_equipbag.h"

extern QVector<Info_equip> g_EquipList;

item_equipBag::item_equipBag(RoleInfo *info, ListEquip *item)
	:  myRole(info), m_item(item)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
}

item_equipBag::~item_equipBag()
{

}

void item_equipBag::updateInfo()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp = "";
	quint32 ID, nCount;
	QString Name;

	//���������������ʾ������ǰװ������С��֮ǰ����ʱ������β��ʾԭװ���ļ���
	ui.tableWidget->clear();
	for (ListEquip::const_iterator iter = m_item->begin(); iter != m_item->end(); iter ++)
	{
		const Info_equip *itemItem = FindItem_Equip(*iter);

		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(QIcon(itemItem->icon), strTmp));
		if (col_cur >= Col_Count)
		{
			++row_cur;
			col_cur = 0;
		}

		if (row_cur >= row_Count)
		{
			//���ӵ��ڶ�ҳ��
			break;	//�ݲ�����
		}
	}
}

void item_equipBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_equip(row, column, m_item, myRole);
}

void item_equipBag::ShowContextMenu(QPoint pos)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, m_item);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	emit wearEquip(ID, index);
}