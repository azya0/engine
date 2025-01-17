#include "editor.h"

std::string getFileData(std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Miss file " + filename);
    }

    std::stringstream sstream;

    sstream << file.rdbuf();
    
    std::string result = sstream.str();

    file.close();

    return result;
}

void updateFile(std::string filename, std::string content) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Miss file " + filename);
    }

    std::stringstream ss;
    ss << content;

    file << ss.rdbuf();

    file.close();
}

std::set<char> const Editor::sentEndSymbs = {'.', '?', '!'};

Editor::Editor(std::string _filename) {
    filename = _filename;
}

void Editor::replace(std::string const & from, std::string const & to) {
    std::string content = getFileData(filename);

    size_t currentPos = 0;
    while ((currentPos = content.find(from, currentPos)) != std::string::npos) {
        content.replace(currentPos, from.length(), to);

        currentPos += to.length();
    }

    updateFile(filename, content);
}

std::vector<std::size_t> Editor::find(std::string const & substr) {
    std::string content = getFileData(filename);

    std::vector<std::size_t> substrStartIndexes;
    size_t currentPos = 0;

    while ((currentPos = content.find(substr, currentPos)) != std::string::npos) {
        substrStartIndexes.push_back(currentPos);
        currentPos += substr.length();
    }

    return substrStartIndexes;
}

bool isSep(char chr) {
    return !(std::isalpha(chr) || std::isdigit(chr));
}

void Editor::capitalizeWords(std::size_t start, std::size_t end) {
    std::string content = getFileData(filename);

    start = std::max(start, 0ull);
    end = std::min(end, content.size());

    while (start > 0 && !isSep(content[start - 1])) {
        start--;
    }

    bool capitalizeNext = true;

    for (std::size_t i = start; i < end; i++) {
        char &c = content[i];
        if (std::isalpha(c) && capitalizeNext) {
            c = static_cast<char>(std::toupper(c));
            capitalizeNext = false;
        } else if (!std::isalpha(c) && !std::isdigit(c)) {
            capitalizeNext = true;
        }
    }

    updateFile(filename, content);
}

bool contains(std::set<char> const & set, char & value) {
    return set.find(value) != set.end();
}

void Editor::capitalizeSentence(std::size_t index) {
    std::string content = getFileData(filename);

    if (contains(sentEndSymbs, content[index]) || index >= content.size()) {
        return;
    }

    while (index > 0 && !contains(sentEndSymbs, content[index - 1])) {
        index--;
    }

    while (index < content.length() && std::isspace(content[index])) {
        index++;
    }
        
    if (index < content.length()) {
        content[index] = static_cast<char>(std::toupper(content[index]));
    }

    updateFile(filename, content);
}

std::string Editor::getContent() {
    return getFileData(filename);
}

std::string::iterator iterFromIndex(std::string &str, std::size_t index) {
    auto iter = str.begin();
    std::advance(iter, index);

    return iter;
}

std::size_t indexFromIter(std::string const & str, std::string::iterator const  iter) {
    int i = 0;
    auto leftIter = str.begin();
    while (leftIter != iter) {
        ++leftIter;
        ++i;
    }

    return i;
}

std::string indexFornat(int index, std::string content) {
    return std::to_string(index) + ". " + content + "\n";
}

void Editor::addSentenceNumbering() {
    std::string content = getFileData(filename);

    static auto findPredicate = [&](char c) -> bool { return contains(sentEndSymbs, c); };

    std::size_t sentenceBeginIndex = 0;
    auto sentenceEnd = content.begin();
    std::size_t number = 1;

    while ((sentenceEnd = std::find_if(sentenceEnd, content.end(), findPredicate)) != content.end()) {
        sentenceEnd++;

        std::size_t sentenceEndIndex = indexFromIter(content, sentenceEnd);

        if (!contains(sentEndSymbs, content[sentenceBeginIndex])) {
            std::string formattedSentence = indexFornat(number, content.substr(sentenceBeginIndex, sentenceEndIndex - sentenceBeginIndex + 1));

            content.replace(sentenceBeginIndex, sentenceEndIndex - sentenceBeginIndex + 1, formattedSentence);

            sentenceEndIndex = sentenceBeginIndex + formattedSentence.length();
            sentenceEnd = iterFromIndex(content, sentenceEndIndex);
            number++;
        }

        sentenceBeginIndex = sentenceEndIndex;
    }

    if (sentenceBeginIndex < content.size() && !contains(sentEndSymbs, content[sentenceBeginIndex])) {
        std::size_t sentenceEndIndex = indexFromIter(content, sentenceEnd);
        
        std::string const formattedSentence = indexFornat(number, content.substr(sentenceBeginIndex, sentenceEndIndex - sentenceBeginIndex + 1));

        content.replace(sentenceBeginIndex, sentenceEndIndex - sentenceBeginIndex + 1, formattedSentence);
    }

    updateFile(filename, content);
}
