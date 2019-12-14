#include <iostream>
#include <stdexcept>

#include "menu.h"

Menu::Menu(){
	this->menu=CreateMenu();
	if(!this->menu)throw (int)GetLastError();
	std::clog<<"[Menu:"<<this->menu<<"] creando menu"<<std::endl;
}

Menu::~Menu(){
	std::clog<<"[Menu:"<<this->menu<<"] destrullendo menu"<<std::endl;
	DestroyMenu(this->menu);
}

void Menu::add(int id,std::string text){
	std::clog<<"[Menu:"<<this->menu<<"] creando boton\n\tid: "<<id<<
			"\n\ttext: "<<text<<std::endl;
	AppendMenu(this->menu, MF_STRING,id,text.c_str());
}

void Menu::add(int id,bool check,std::string text){
	std::clog<<"[Menu:"<<this->menu<<"] creando check\n\tid: "<<id<<
			"\n\tcheck: "<<check<<
			"\n\ttext: "<<text<<std::endl;
	AppendMenu(this->menu,check?MF_CHECKED:MF_UNCHECKED,id,text.c_str());
}

void Menu::add(std::shared_ptr<Menu> &subMenu,std::string text){
	std::clog<<"[Menu:"<<this->menu<<"] creando submenu\n\tpadre: "<<subMenu->get()<<
			"\n\ttext: "<<text<<std::endl;
	if(AppendMenu(this->menu, MF_POPUP,(UINT_PTR)subMenu->get(),text.c_str())){
		this->subMenus.push_back(subMenu);
	}
}

void Menu::setChack(int id,bool check){
	CheckMenuItem(this->menu,id,check?MF_CHECKED:MF_UNCHECKED);
}

HMENU Menu::get(){
	return this->menu;
}
