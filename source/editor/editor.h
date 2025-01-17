#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <set>


class Editor {
private:
    std::string filename;
public:
    static std::set<char> const sentEndSymbs;

    Editor(std::string filename);

    void replace(std::string const & from, std::string const & to);

    std::vector<std::size_t> find(std::string const & substr);

    void capitalizeWords(std::size_t start, std::size_t end);

    void capitalizeSentence(std::size_t index);

    std::string getContent();

    void addSentenceNumbering();
};

