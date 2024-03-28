/*!
* @brief   
* @project plugincore
* @file    fifo.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-20 19:17
*/
//==============================================================================
#ifndef PLUGINCORE_FIFO_H
#define PLUGINCORE_FIFO_H
//==============================================================================

#include <mutex>
#include <deque>


template<typename T>
class FIFO
{

public:
	~FIFO() {}

	FIFO() {}
	FIFO(size_t capacity) : m_capacity(capacity < 0 ? -1 : capacity) {}

	FIFO(const FIFO& ohter) = delete;
	FIFO& operator=(const FIFO& other) = delete;

	void close()
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_closed = true;
		m_cond.notify_all();
	}

	size_t capacity() 
	{
		return m_capacity;
	}

	size_t size() 
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		return m_deque.size();
	}

	void clear()
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_deque.clear();
	}

	bool push(const T& data)
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		if (!closed() && !full())
		{
			m_deque.push_back(data);
			m_cond.notify_one();
		}
		else
		{
			return false;
		}
		return true;
	}

	bool pop(T& data)
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		m_cond.wait(lock, [this]() { return !empty() || m_closed; });
		if (!empty())
		{
			data = m_deque.front();
			m_deque.pop_front();
		}
		else if (m_closed)
		{
			return false;
		}
		return true;
	}

private:
	bool empty()
	{
		return m_deque.empty();
	}

	bool full()
	{
		return m_capacity == -1 ? false : m_deque.size() >= m_capacity;
	}

	bool closed()
	{
		return m_closed;
	}

private:
	std::mutex m_mtx;
	std::condition_variable m_cond;
	size_t m_capacity = -1;
	bool m_closed = false;
	std::deque<T> m_deque;
};


//==============================================================================
#endif // PLUGINCORE_FIFO_H
//==============================================================================
