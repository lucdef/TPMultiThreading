#pragma once

#include "CSocketIp4.h"


class OGlobal;
class LogManager;

class TcpServer
{
	public:
		TcpServer();
		~TcpServer(){}
		//int StartServer();
		
		/*
		 * \brief Lance le server sur le port designe
		 *
		 * Fonctionnement
		 * Tant que _isRunning
		 *     recevoir data -> data
		 *     parser data recu -> reponse
		 *     envoyer reponse -> isRunning
		 *
		 * \param port Le port sur lequel lancer le serveur
		 */
		void Run(unsigned short port);

		void StopServer();

		

		/* TESTS */
		static std::string TestGetPassFromData();
		static std::string TestGetFounderFromData();
		/* ------*/
	private:
		static const short MAX_CONNECTION = 10;
		static const std::string PASS_PATTERN;
		static const std::string FOUNDER_PATTERN;

		LogManager* const _logger;

		CSocketIp4 _socket;
		CSocketIp4 *_remoteClient;
		char _buffer[1024];
		int _recvCount;
		std::string _request;
		std::string _response;
		bool _isRunning;

		/**
		* \brief Parse la string data et fabrique la reponse en fonction.
		* \param data la string de donnees recue
		* \return La reponse fabriquee
		*/
		std::string ParseHttp(const std::string &data) const;

		/**
		* \brief Ecoute le reseau en attente d'une query du client.
		* \return La data recue
		*/
		std::string ReceiveData();

		bool SendData(const std::string & data) const;

		/**
		* \brief Envoi les data au client.
		* \param data les datas a envoyer
		* \return true si ok, false sinon
		*/
		bool SendData(const std::string &data) const;

		/**
		* \brief Deconnecte un client.
		*/
		void DisconnectClient();
};

