/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "Network.h"

#ifdef NETLIB_EPOLL

epollEngine::epollEngine()
{
	new SocketDeleter();
	epoll_fd = epoll_create(MAX_DESCRIPTORS);
	assert(epoll_fd != -1);
	memset(this->fds, 0, sizeof(void*) * MAX_DESCRIPTORS);
	m_running = true;
}

epollEngine::~epollEngine()
{
	close(epoll_fd);
}

void epollEngine::AddSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == 0);
	fds[s->GetFd()] = s;

	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = (s->Writable()) ? EPOLLOUT : EPOLLIN;
	ev.events |= EPOLLET;

	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, s->GetFd(), &ev);
}

void epollEngine::RemoveSocket(BaseSocket * s)
{
	assert(fds[s->GetFd()] == s);
	fds[s->GetFd()] = 0;

	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = (s->Writable()) ? EPOLLOUT : EPOLLIN;
	ev.events |= EPOLLET;

	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, s->GetFd(), &ev);
}

void epollEngine::WantWrite(BaseSocket * s)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	ev.data.fd = s->GetFd();
	ev.events = EPOLLOUT | EPOLLET;

	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, s->GetFd(), &ev);
}

void epollEngine::MessageLoop()
{
	const static int maxevents = 1024;
	struct epoll_event events[1024];
	int nfds, i;
	BaseSocket * s;
	while(m_running)
	{
        	nfds = epoll_wait(epoll_fd, events, maxevents, 1000);
		for(i = 0; i < nfds; ++i)
		{
	            s = fds[events[i].data.fd];
			if(s == 0)
			{
				printf("epoll returned invalid fd %u\n", events[i].data.fd);
				continue;
			}

			if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR)
			{
				s->OnError(errno);
			}
			else if(events[i].events & EPOLLIN)
			{
				s->OnRead(0);
				if(s->Writable() && !s->m_writeLock)
				{
					++s->m_writeLock;
					WantWrite(s);
				}
			}
			else if(events[i].events & EPOLLOUT)
			{
				s->OnWrite(0);
				if(!s->Writable())
				{
					/* change back to read state */
					struct epoll_event ev;
					memset(&ev, 0, sizeof(epoll_event));
					ev.data.fd = s->GetFd();
					ev.events = EPOLLIN | EPOLLET;

					epoll_ctl(epoll_fd, EPOLL_CTL_MOD, s->GetFd(), &ev);
					--s->m_writeLock;
				}
			}
		}
	}
}

void epollEngine::Shutdown()
{
	m_running = false;
	for(int i = 0; i < MAX_DESCRIPTORS; ++i)
	{
		if(fds[i] != 0)
		{
			fds[i]->Delete();
		}
	}

	sSocketDeleter.Kill();
	delete SocketDeleter::getSingletonPtr();
	delete this;
}

void epollEngine::SpawnThreads()
{
	launch_thread(new SocketEngineThread(this));
}

#endif
