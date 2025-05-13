#pragma once

void MakeCoin(CImage* Coinimg, vector<Object*>& coins)
{
	Object* coin_1 = new Coin{};
	coin_1->SetPos(Vec2(1430.f, 250.f));
	coin_1->Setting();
	coin_1->Setimg(Coinimg);
	coins.push_back(coin_1);

	Object* coin_2 = new Coin{};
	coin_2->SetPos(Vec2(540.f, 250.f));
	coin_2->Setting();
	coin_2->Setimg(Coinimg);
	coins.push_back(coin_2);

	Object* coin_3 = new Coin{};
	coin_3->SetPos(Vec2(2730.f, 250.f));
	coin_3->Setting();
	coin_3->Setimg(Coinimg);
	coins.push_back(coin_3);


	Object* coin_4 = new Coin{};
	coin_4->SetPos(Vec2(3600.f, 250.f));
	coin_4->Setting();
	coin_4->Setimg(Coinimg);
	coins.push_back(coin_4);
}

void MakeThorn(CImage* Thimg, vector<Object*>& thorns)
{
	Object* thorn_1 = new Thorn;
	thorn_1->SetPos(Vec2(1315.f, 255.f));
	thorn_1->SetScale(Vec2(95.f, 50.f));
	thorn_1->Setting();
	thorn_1->Setimg(Thimg);
	thorns.push_back(thorn_1);

	Object* thorn_2 = new Thorn;
	thorn_2->SetPos(Vec2(630.f, 260.f));
	thorn_2->SetScale(Vec2(90.f, 50.f));
	thorn_2->Setting();
	thorn_2->Setimg(Thimg);
	thorns.push_back(thorn_2);

	Object* thorn_3 = new Thorn;
	thorn_3->SetPos(Vec2(2830.f, 255.f));
	thorn_3->SetScale(Vec2(95.f, 50.f));
	thorn_3->Setting();
	thorn_3->Setimg(Thimg);
	thorns.push_back(thorn_3);


	Object* thorn_4 = new Thorn;
	thorn_4->SetPos(Vec2(3515.f, 260.f));
	thorn_4->SetScale(Vec2(90.f, 50.f));
	thorn_4->Setting();
	thorn_4->Setimg(Thimg);
	thorns.push_back(thorn_4);
}


void Makemonsters(CImage* Monimg, vector<Monster*>& monsters)
{


	Monster* monster_2 = new Monster{};
	monster_2->SetPos(Vec2(700.f, 600.f));
	monster_2->Setting();
	monster_2->Setimg(Monimg);
	monsters.push_back(monster_2);


	Monster* monster_4 = new Monster{};
	monster_4->SetPos(Vec2(3000.f, 500.f));
	monster_4->Setting();
	monster_4->Setimg(Monimg);
	monsters.push_back(monster_4);
}


void LoadMap_1(vector<Object*>& Grounds, RECT Clientrect,CImage* img)
{

	RECT map[100]{};

	map[0].left = Clientrect.left; // ¶¥1
	map[0].right = 104;
	map[0].top = Clientrect.bottom - 187;
	map[0].bottom = Clientrect.bottom;

	Object* pground_1 = new Ground;
	pground_1->SetObject(map[0]);
	pground_1->Setting();
	Grounds.push_back(pground_1);

	map[1].left = 104; // ¶¥2
	map[1].right = 104 + 282;
	map[1].top = Clientrect.bottom - 114;
	map[1].bottom = Clientrect.bottom;

	Object* pground_2 = new Ground;
	pground_2->SetObject(map[1]);
	pground_2->Setting();
	Grounds.push_back(pground_2);


	map[2].left = 386; // ¾ð´ö¶¥3 
	map[2].right = 386 + 213;
	map[2].top = Clientrect.bottom - 213;
	map[2].bottom = Clientrect.bottom;

	Object* pground_3 = new Ground;
	pground_3->SetObject(map[2]);
	pground_3->Setting();
	Grounds.push_back(pground_3);


	map[3].left = 599; // ÆòÁö¶¥4
	map[3].right = 599 + 968;
	map[3].top = Clientrect.bottom - 135;
	map[3].bottom = Clientrect.bottom;

	Object* pground_4 = new Ground;
	pground_4->SetObject(map[3]);
	pground_4->Setting();
	Grounds.push_back(pground_4);

	map[4].left = 1567; // ´ÙÀ½¸ÊÀÕ´Â¶¥5
	map[4].right = 1567 + 1012;
	map[4].top = Clientrect.bottom - 211;
	map[4].bottom = Clientrect.bottom;

	Object* pground_5 = new Ground;
	pground_5->SetObject(map[4]);
	pground_5->Setting();
	pground_5->SetIce(true);
	pground_5->Setimg(img);
	Grounds.push_back(pground_5);

	map[5].left = 0; // ¿ÞÂÊ ÁöºØ1
	map[5].right = 314;
	map[5].top = 0;
	map[5].bottom = 313;

	Object* pground_6 = new Ground;
	pground_6->SetObject(map[5]);
	pground_6->Setting();
	Grounds.push_back(pground_6);

	map[6].left = 314; // ¿ÞÂÊ ÁöºØ2
	map[6].right = 314 + 100;
	map[6].top = 0;
	map[6].bottom = 183;

	Object* pground_7 = new Ground;
	pground_7->SetObject(map[6]);
	pground_7->Setting();
	Grounds.push_back(pground_7);

	map[7].left = 414; // ¿ÞÂÊ ÁöºØ3
	map[7].right = 414 + 1052;
	map[7].top = 0;
	map[7].bottom = 54;

	Object* pground_8 = new Ground;
	pground_8->SetObject(map[7]);
	pground_8->Setting();
	Grounds.push_back(pground_8);

	map[8].left = 438; // ¾ð´ö¶¥À§°øÁß¶¥1
	map[8].right = 438 + 241;
	map[8].top = 285;
	map[8].bottom = 285 + 89;

	Object* pground_9 = new Ground;
	pground_9->SetObject(map[8]);
	pground_9->Setting();
	Grounds.push_back(pground_9);

	map[9].left = 723; // °øÁß2 
	map[9].right = 723 + 57;
	map[9].top = 360;
	map[9].bottom = 360 + 55;

	Object* pground_10 = new Ground;
	pground_10->SetObject(map[9]);
	pground_10->Setting();
	Grounds.push_back(pground_10);

	map[10].left = 809; // °øÁß3
	map[10].right = 809 + 102;
	map[10].top = 470;
	map[10].bottom = 470 + 27;

	Object* pground_11 = new Ground;
	pground_11->SetObject(map[10]);
	pground_11->Setting();
	Grounds.push_back(pground_11);

	map[11].left = 973; //Ç®°øÁß4
	map[11].right = 973 + 52;
	map[11].top = 390;
	map[11].bottom = 390 + 53;

	Object* pground_12 = new Ground;
	pground_12->SetObject(map[11]);
	pground_12->Setting();
	Grounds.push_back(pground_12);

	map[12].left = 1261; //°øÁßÆòÁö4
	map[12].right = 1261 + 233;
	map[12].top = 280;
	map[12].bottom = 280 + 76;

	Object* pground_13 = new Ground;
	pground_13->SetObject(map[12]);
	pground_13->Setting();
	Grounds.push_back(pground_13);

	map[13].left = 1466; //°øÁßÆòÁöº®4
	map[13].right = 1466 + 257;
	map[13].top = 0;
	map[13].bottom = 280;

	Object* pground_14 = new Ground;
	pground_14->SetObject(map[13]);
	pground_14->Setting();
	Grounds.push_back(pground_14);

	map[14].left = 1723; //´ÙÀ½¸Ê ÀÕ´Â º®4
	map[14].right = 1723 + 700;
	map[14].top = 0;
	map[14].bottom = 420;

	Object* pground_15 = new Ground;
	pground_15->SetObject(map[14]);
	pground_15->Setting();
	Grounds.push_back(pground_15);

	map[15].left = 2579; //´ÙÀ½¸Ê¶¥1
	map[15].right = 2579 + 967;
	map[15].top = Clientrect.bottom - 134;
	map[15].bottom = Clientrect.bottom;

	Object* pground_16 = new Ground;
	pground_16->SetObject(map[15]);
	pground_16->Setting();
	Grounds.push_back(pground_16);

	map[16].left = 3546; //´ÙÀ½¸Ê¶¥2
	map[16].right = 3546 + 215;
	map[16].top = Clientrect.bottom - 210;
	map[16].bottom = Clientrect.bottom;

	Object* pground_17 = new Ground;
	pground_17->SetObject(map[16]);
	pground_17->Setting();
	Grounds.push_back(pground_17);

	map[17].left = 3761; //´ÙÀ½¸Ê¶¥3
	map[17].right = 3761 + 278;
	map[17].top = Clientrect.bottom - 110;
	map[17].bottom = Clientrect.bottom;

	Object* pground_18 = new Ground;
	pground_18->SetObject(map[17]);
	pground_18->Setting();
	Grounds.push_back(pground_18);

	map[18].left = 4039; //´ÙÀ½¸Ê¶¥4
	map[18].right = 4039 + 108;
	map[18].top = Clientrect.bottom - 183;
	map[18].bottom = Clientrect.bottom;

	Object* pground_19 = new Ground;
	pground_19->SetObject(map[18]);
	pground_19->Setting();
	Grounds.push_back(pground_19);

	map[19].left = 2423; //´ÙÀ½¸ÊÆòÁöº®
	map[19].right = 2423 + 257;
	map[19].top = 0;
	map[19].bottom = 280;

	Object* pground_20 = new Ground;
	pground_20->SetObject(map[19]);
	pground_20->Setting();
	Grounds.push_back(pground_20);

	map[20].left = 2680; //´ÙÀ½¸ÊÁöºØ1
	map[20].right = 2680 + 1052;
	map[20].top = 0;
	map[20].bottom = 54;

	Object* pground_21 = new Ground;
	pground_21->SetObject(map[20]);
	pground_21->Setting();
	Grounds.push_back(pground_21);

	map[21].left = 3732; //´ÙÀ½¸ÊÁöºØ2
	map[21].right = 3732 + 100;
	map[21].top = 0;
	map[21].bottom = 183;

	Object* pground_22 = new Ground;
	pground_22->SetObject(map[21]);
	pground_22->Setting();
	Grounds.push_back(pground_22);

	map[23].left = 3832; //´ÙÀ½¸ÊÁöºØ3
	map[23].right = 3832 + 314;
	map[23].top = 0;
	map[23].bottom = 313;

	Object* pground_23 = new Ground;
	pground_23->SetObject(map[22]);
	pground_23->Setting();
	Grounds.push_back(pground_23);

	map[24].left = 2652; //´ÙÀ½¸Ê°øÁßÆòÁö
	map[24].right = 2652 + 233;
	map[24].top = 280;
	map[24].bottom = 280 + 76;

	Object* pground_24 = new Ground;
	pground_24->SetObject(map[23]);
	pground_24->Setting();
	Grounds.push_back(pground_24);

	map[25].left = 3125; //´ÙÀ½¸ÊÇ®°øÁß1
	map[25].right = 3125 + 52;
	map[25].top = 390;
	map[25].bottom = 390 + 53;

	Object* pground_25 = new Ground;
	pground_25->SetObject(map[24]);
	pground_25->Setting();
	Grounds.push_back(pground_25);

	map[26].left = 3235; // °øÁß2
	map[26].right = 3235 + 102;
	map[26].top = 470;
	map[26].bottom = 470 + 27;

	Object* pground_26 = new Ground;
	pground_26->SetObject(map[25]);
	pground_26->Setting();
	Grounds.push_back(pground_26);

	map[27].left = 3370; // °øÁß2 
	map[27].right = 3370 + 57;
	map[27].top = 360;
	map[27].bottom = 360 + 55;

	Object* pground_27 = new Ground;
	pground_27->SetObject(map[26]);
	pground_27->Setting();
	Grounds.push_back(pground_27);

	map[28].left = 3470; // ¾ð´ö¶¥À§°øÁß¶¥1
	map[28].right = 3470 + 241;
	map[28].top = 285;
	map[28].bottom = 285 + 89;

	Object* pground_28 = new Ground;
	pground_28->SetObject(map[27]);
	pground_28->Setting();
	Grounds.push_back(pground_28);


	map[98].left = Clientrect.left - 512; // ¾ð´ö¶¥À§°øÁß¶¥1
	map[98].right = Clientrect.left;
	map[98].top = 0;
	map[98].bottom = Clientrect.bottom;

	Object* pground_29 = new Ground;
	pground_29->SetObject(map[28]);
	pground_29->Setting();
	Grounds.push_back(pground_29);

	map[99].left = 4147; // ¾ð´ö¶¥À§°øÁß¶¥1
	map[99].right = 4147 + 512;
	map[99].top = 0;
	map[99].bottom = Clientrect.bottom;

	Object* pground_30 = new Ground;
	pground_30->SetObject(map[29]);
	pground_30->Setting();
	Grounds.push_back(pground_30);

}

void LoadMap_2(vector<Object*>& Grounds, RECT Clientrect)
{
	RECT map[2]{};

	map[0].left = -300; // ¶¥1
	map[0].right = 6000;
	map[0].top = Clientrect.bottom - 100;
	map[0].bottom = Clientrect.bottom;

	Object* pground_1 = new Ground;
	pground_1->SetObject(map[0]);
	pground_1->Setting();
	Grounds.push_back(pground_1);



}

void Init_1(vector<Object*>& Grounds, vector<Object*>& coins,
	vector<Monster*>& monsters, vector<Object*>& thorns, CImage& backimg, RECT Clinetrect)
{
	for (int i{}; i < Grounds.size(); ++i)
	{
		delete Grounds.at(i);
	}
	for (int i{}; i < coins.size(); ++i)
	{
		delete coins.at(i);
	}
	for (int i{}; i < monsters.size(); ++i)
	{
		delete monsters.at(i);
	}
	for (int i{}; i < thorns.size(); ++i)
	{
		delete thorns.at(i);
	}
	Grounds.clear();
	coins.clear();
	monsters.clear();
	thorns.clear();

}

void Init_2(vector<Object*>& Grounds, vector<Fire*>& fires,
	vector<Monster*>& monsters)
{
	for (int i{}; i < Grounds.size(); ++i)
	{
		delete Grounds.at(i);
	}
	for (int i{}; i < fires.size(); ++i)
	{
		delete fires.at(i);
	}
	for (int i{}; i < monsters.size(); ++i)
	{
		delete monsters.at(i);
	}

	Grounds.clear();
	fires.clear();
	monsters.clear();

}

void NextStage(vector<Object*>& Grounds, vector<Object*>& coins, 
	vector<Monster*>& monsters, vector<Object*>& thorns, CImage& backimg,RECT Clinetrect)
{
	for (int i{}; i<Grounds.size(); ++i)
	{
		delete Grounds.at(i);
	}
	for (int i{}; i < coins.size(); ++i)
	{
		delete coins.at(i);
	}
	for (int i{}; i < monsters.size(); ++i)
	{
		delete monsters.at(i);
	}
	for (int i{}; i < thorns.size(); ++i)
	{
		delete thorns.at(i);
	}
	Grounds.clear();
	coins.clear();
	monsters.clear();
	thorns.clear();

	backimg.Destroy();
	backimg.Load(TEXT("image\\backimg.png"));

	LoadMap_2(Grounds, Clinetrect);
}

