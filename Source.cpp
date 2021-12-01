#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>   

struct Element
{
    //std::string name;
    int votes;
    int rank;
};

void AssignRank(std::vector<Element> &listOfElements)
{
    // Create an temp array of references to objects
    size_t size = listOfElements.size();
    std::vector<Element *> tempArray(size);

    for (size_t i = 0; i < size; ++i)
    {
        tempArray[i] = &listOfElements[i];
    }


    // Sort temp array
    std::sort(tempArray.begin(), tempArray.end(), [](Element *lhs, Element *rhs) {
        return lhs->votes > rhs->votes;
        });

    // Assign rank
    int rank = 0;
    for (auto element : tempArray)
    {
        element->rank = rank++;
    }

}

int main()
{
    std::vector<Element> elementList(100);

    // Create a list of elements with votes 0-99 ranked 99-0
    int currRank = 0;
    int currVotes = 99;
    for (auto &element : elementList)
    {
        element = Element{ currVotes--, currRank++ };
    }

    // Swap a few elements
    for (int i = 0; i < 20; ++i)
    {
        std::swap(elementList[rand() % 100], elementList[rand() % 100]);
    }

    // Clone the element list without the ranks
    std::vector<Element> elementListUnderTest(100);
    int index = 0;
    for (auto &element : elementListUnderTest)
    {
        element = Element{ elementList[index++].votes, -1 };
    }

    //for (auto element : elementListUnderTest)
    //{
    //    printf("%d \n", element.votes);
    //}

    AssignRank(elementListUnderTest);

    

    // Compare two lists
    for (int i = 0; i < 100; ++i)
    {
        if (elementListUnderTest[i].rank != elementList[i].rank)
        {
            printf("Test failed \n");
            return 1;
        }
    }

    printf("Test passed \n");


    return 0;
}