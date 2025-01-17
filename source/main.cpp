#include "editor/editor.h"
#include "wrapper/wrapper.h"
#include "engine/engine.h"

#include <iostream>

int main() {
    Editor editor("test.txt");

    Wrapper numbering_wrapper(&editor, &Editor::addSentenceNumbering, {});
    Wrapper replacing_wrapper(&editor, &Editor::replace, {"from", "to"});
    Wrapper substring_wrapper(&editor, &Editor::find, {"substring"});
    Wrapper content_wrapper(&editor, &Editor::getContent, {});
    Wrapper cap_words_wrapper(&editor, &Editor::capitalizeWords, {"from", "to"});
    Wrapper cap_sent_wrapper(&editor, &Editor::capitalizeSentence, {"from", "to"});

    Engine engine;

    engine.register_command(&numbering_wrapper, "Number");
    engine.register_command(&replacing_wrapper, "Replace");
    engine.register_command(&substring_wrapper, "Find substring");
    engine.register_command(&content_wrapper, "Content");
    engine.register_command(&cap_words_wrapper, "Capitalize words");
    engine.register_command(&cap_sent_wrapper, "Capitalize sentence");

    engine.execute<void>("Number", {});
    engine.execute<void>("Capitalize words", {{"from", 0ull}, {"to", 20ull}});
    engine.execute<void>("Capitalize sentence", {{"from", 0ull}, {"to", 20ull}});
    engine.execute<void>("Replace", {{"from", std::string("aaa")}, {"to", std::string("there were \"aaa\"")}});

    auto indexes = engine.execute<std::vector<std::size_t>>("Find substring", {{"substring", std::string("aaa")}});

    for (auto value : indexes) {
        std::cout << value << '\n';
    }

    std::cout << engine.execute<std::string>("Content", {});

    try {
        engine.execute<void>("Flexer", {});
    } catch(std::exception& error) {
        std::cout << error.what() << '\n';
    }
    
    return 0;
}
