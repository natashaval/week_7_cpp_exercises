#include <ostream>
#include <stdexcept>
#include <vector>

using std::vector;

template<typename keyType, typename valType>
class SortedList
{
    struct kv_pair
    {
        keyType key;
        valType value;
    };

    public:
    SortedList(){}

    /**
     * @brief Insertion into sorted list. 
     * 
     * @param key 
     * @param value 
     * 
     * O(n) for SortedList because iterate over O(n) + insert&shift for existing O(n)
     */
    void insert(const keyType key, const valType value)
    {
        if(list.empty())
        {
            list.push_back(kv_pair{key, value});
        }
        else
        {
            for (size_t i = list.size() - 1; i > 0; i--)
            {
                if (key == list[i].key)
                {
                    list[i].value = value;
                    return;
                }
                else if (key > list[i].key)
                {
                    return insertAndShift(key, value, i+1);
                }
            }
            key > list[0].key ? insertAndShift(key, value, 1) : insertAndShift(key, value, 0);
        }
    }

    /**
     * @brief Lookup by binary search algorithm
     * 
     * @param key 
     * @return valType 
     * 
     * O(log n) because binary search split into 2 each time
     */
    valType lookup(keyType key)
    {
        if(list.empty()) throw std::runtime_error("Key not found.");

        size_t i_max = list.size()-1;
        size_t i_min = 0;
        size_t i = list.size()/2;
        while(i_min != i_max)
        {
            if(key == list[i].key)
            {
                return list[i].value;
            }
            else if(key < list[i].key)
            {
                i_max = i-1;
                i = i_min + (i_max - i_min)/2;
            }
            else
            {
                i_min = i+1;
                i = i_min + (i_max - i_min)/2;
            }
        }

        // only one value left
        if(key == list[i].key)
        {
            return list[i].value;
        }
        else
        {
            throw std::runtime_error("Key not found.");
        }
    }

    friend std::ostream &operator<<(std::ostream &out, SortedList &L)
    {
        out << "[";
        for(const auto &x : L.list)
        {
            out << "(" << x.key << "," << x.value << ")" << " ";
        }
        out << "]";
        return out;
    }

    private:
    // This method can be replaced by std lib vector::insert with the same 
    // complexity but we have written it out explicitly so you can see what is happening.
    void insertAndShift(const keyType key, const valType value, size_t i)
    {
        kv_pair buffer_pair;
        kv_pair insertion_pair{key, value};

        while(i < list.size())
        {
            buffer_pair = list[i];
            list[i] = insertion_pair;
            insertion_pair = buffer_pair;
            i++;
        }
        list.push_back(insertion_pair);
    }

    vector<kv_pair> list;
};