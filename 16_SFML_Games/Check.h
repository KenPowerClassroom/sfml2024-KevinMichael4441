#pragma once
#include <memory>
#include <vector>


struct Point
{
    int x, y;
};


class Check
{
public:

    Check(Point t_actual[], int t_sizeofActual, std::vector<std::vector<int>> t_field)
    {
        m_field = t_field;

        m_actual.resize(t_sizeofActual);
        for (int index = 0; index < t_sizeofActual; index++)
        {
            m_actual[index] = t_actual[index];
        }
    }

    bool runningTheCheck()
    {
        for (int i = 0; i < m_actual.size(); i++)
        {
            if (m_actual[i].x < 0 || m_actual[i].x >= m_WIDTH || m_actual[i].y >= m_HEIGHT)
            {
                return false;
            }
            else if (m_field[m_actual[i].y][m_actual[i].x])
            {
                return false;
            }
        }

        return  true;
    }


private:
    int const m_WIDTH = 10;
    int const m_HEIGHT = 20;

    std::vector<Point> m_actual;
    std::vector<std::vector<int>> m_field;

};
