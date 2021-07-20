#pragma once

namespace SGA
{
    template<typename Container>
    class IteratorProxy
    {
    public:
        IteratorProxy(Container& container) : container(container)
        { 
        }

        auto begin()
        {
            return container.begin();
        }

        auto end()
        {
            return container.end();
        }
    	
        auto begin() const
    	{
            return container.begin();
        }
    	
        auto end() const
    	{
            return container.end();
        }
    	
    private:
        Container& container;
    };
	
    template<typename Container>
    class ConstIteratorProxy
    {
    public:
        ConstIteratorProxy(const Container& container) : container(container)
        {
        }
    	
        auto begin() const
    	{
            return container.begin();
        };
    	
        auto end() const
    	{
            return container.end();
        };
    private:
        const Container& container;
    };
}