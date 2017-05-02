#include "server.hpp"
#include <boost/array.hpp>

namespace server_sea_battle
{
server::server() : message( "start" ),
                   service( new boost::asio::io_service() ),
                   socket( new boost::asio::ip::tcp::socket(*service) ),
                   acceptor( new boost::asio::ip::tcp::acceptor( *service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), PORT ) ) )
{
   std::cout<<"Initialization fields - OK"<<std::endl;
}

// Запускает сервер. В методе крутится вся логика сервера.
void server::StartServer()
{
   while(true)
   {
      try
      {
         std::cout<<"Start server"<<std::endl;
         Acception(); // Ожидаем подключения
         if(message!="start") // В случае если это первое подключение
         {
            if(!IsEndGame())
            {
               SetMessage();
            }
            GetMessage();
         }
         else
         {
            GetMessage();
            IsEndGame();
         }
         socket->close();
      }
      catch(...)
      {
         IsEndGame();
         socket->close();
      }
   }
}
// Получаем и обрабатываем сообщение от клиента
void server::GetMessage()
{
   std::cout<<"Get message"<<std::endl;
   boost::array<char, 3> buffer; // Создаем буфер для записи в него сообщения
   //socket->read_some(boost::asio::buffer(buffer,3));
   boost::asio::read( *socket, boost::asio::buffer(buffer,3), boost::asio::transfer_all()); // Читаем сообщение клиентa
   WriteInMessage(buffer.data(), buffer.size());
   std::cout<<"Message: "<<message<<std::endl;
}
// Отправляет содержимое переменной message клиенту
void server::SetMessage()
{
   std::cout<<"Set message"<<std::endl;
   boost::array<std::string,1> buffer; //Создали буфер для хранения сообщения.
   buffer[0]=message;
   boost::asio::write(*socket,boost::asio::buffer(buffer[0])); //Отправляем сообщение
}
// Ожидает подключения клиента
void server::Acception()
{
   std::cout<<"Acception...."<<std::endl;
   acceptor->accept( *socket );
   std::cout<<"Connection received"<<std::endl;
}
// Записывает в переменную message элементы массива char
void server::WriteInMessage(char* data, int size)
{
   message="";
   for(int index_element=0; index_element<size; index_element++)
   {
      message+=data[index_element];
   }
}
// Узнает состояние игры (сообщения могут быть двух типов "число,число" и "end" - конец игры)
bool server::IsEndGame()
{
   bool result = false;
   if(message=="end") // Если мы получили сообщение "end" мы должны сбросить сервер в состояние, как будто мы только начали работу
   {
      message="start";
      result=true;
      std::cout<<"IsEndGame - true"<<std::endl;
   }
   else
   {
      std::cout<<"IsEndGame - false"<<std::endl;
   }
   return result;
}

}
