#include <string>
#include <sstream>
#include <list>

#include "test_runner.h"
#include "profile.h"

using namespace std;

class Editor {
public:
  Editor();
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;

private:
  list<char> text;
  list<char>::iterator cursor_pos = text.begin();
  list<char> buffer;

  auto MovePosition(size_t tokens = 1);
};

Editor::Editor() {}

auto Editor::MovePosition(size_t tokens) {
  auto move_pos = cursor_pos;
  for (int i = 0; i < tokens; ++i) {
    if (move_pos == text.end()) {
      break;
    }
    ++move_pos;
  }
  return move_pos;
}

void Editor::Left() {
  if (cursor_pos != text.begin()) {
    --cursor_pos;
  }
}

void Editor::Right() {
  if (cursor_pos != text.end()) {
    ++cursor_pos;
  }
}

void Editor::Insert(char token) {
  text.insert(cursor_pos, token);
}

void Editor::Cut(size_t tokens) {
  buffer.clear();
  auto it = MovePosition(tokens);
  buffer.splice(buffer.begin(), text, cursor_pos, it);
  cursor_pos = it;
}

void Editor::Copy(size_t tokens) {
  buffer.assign(cursor_pos, MovePosition(tokens));
}

void Editor::Paste() {
  text.insert(cursor_pos, buffer.begin(), buffer.end());
}

string Editor::GetText() const {
  ostringstream os;
  for (auto it = text.begin(); it != text.end(); ++it) {
    os << *it;
  }
  return os.str();
}

// Unit tests

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestCopyPaste() {
  Editor editor;
  const size_t text_len = 12;
  TypeText(editor, "hello, world");

  for(size_t i = 0; i < text_len; ++i) {
    editor.Left();
  }

  editor.Copy(5);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "hellohello, world");

  editor.Copy(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "hellohello, world");

  editor.Copy(14);
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "hellohello, worldhello, world");

  editor.Copy();
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "hellohello, worldhhello, world");

  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "hellohello, worldhhhello, world");

  for (const char c : editor.GetText()) {
    editor.Right();
  }

  editor.Copy();
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "hellohello, worldhhhello, world");

  editor.Insert('!');
  editor.Paste();
  ASSERT_EQUAL(editor.GetText(), "hellohello, worldhhhello, world!");
}

void TestEditing() {
  {
    Editor editor;
    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);

    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;

    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

void TestDefault() {
  Editor editor;
  const string text = "hello, world";
  for (char c : text) {
    editor.Insert(c);
  }
  // Текущее состояние редактора: `hello, world|`
  for (size_t i = 0; i < text.size(); ++i) {
    editor.Left();
  }
  // Текущее состояние редактора: `|hello, world`
  editor.Cut(7);
  // Текущее состояние редактора: `|world`
  // в буфере обмена находится текст `hello, `
  for (size_t i = 0; i < 5; ++i) {
    editor.Right();
  }
  // Текущее состояние редактора: `world|`
  editor.Insert(',');
  editor.Insert(' ');
  // Текущее состояние редактора: `world, |`
  editor.Paste();
  // Текущее состояние редактора: `world, hello, |`
  editor.Left();
  editor.Left();
  //Текущее состояние редактора: `world, hello|, `
  editor.Cut(3); // Будут вырезаны 2 символа
  // Текущее состояние редактора: `world, hello|`
  ASSERT_EQUAL(editor.GetText(), string("world, hello"));
}

string GenerateText() {
   const int SIZE = 1000000;
   const int WORD_LENGTH = 1000000000;
   string text(SIZE, 'a');
   for (int i = WORD_LENGTH; i < SIZE; i += WORD_LENGTH) {
     text[i] = ' ';
   }
   return text;
 }

const string TEXT = GenerateText();

void StressTestInsertGet() {
  Editor editor;
  {
    LOG_DURATION("Insert");
    TypeText(editor, TEXT);
  }
  {
    LOG_DURATION("GetText");
    editor.GetText();
  }
}

void StressTestCutPaste() {
  Editor editor;
  TypeText(editor, TEXT);
  {
    LOG_DURATION("Cut/Paste 100");
    for (int i = 0; i < 50000; ++i) {
      editor.Cut(100);
      editor.Paste();
    }
  }

  Editor editor2;
  TypeText(editor2, TEXT);
  {
    LOG_DURATION("Cut/Paste 1");
    for (int i = 0; i < 1000000; ++i) {
      editor2.Cut(1);
      editor2.Paste();
    }
  }
}

void StressTestCut() {
  Editor editor;
  TypeText(editor, TEXT);
  {
    LOG_DURATION("Cut 100");
    for (int i = 0; i < 50000; ++i) {
      editor.Cut(100);
    }
  }

  Editor editor2;
  TypeText(editor2, TEXT);
  {
    LOG_DURATION("Cut 1");
    for (int i = 0; i < 1000000; ++i) {
      editor2.Cut(1);
    }
  }
}

void StressTestCopy() {
  Editor editor;
  TypeText(editor, TEXT);
  {
    LOG_DURATION("Copy 100");
    for (int i = 0; i < 50000; ++i) {
      editor.Copy(100);
    }
  }

  Editor editor2;
  TypeText(editor2, TEXT);
  {
    LOG_DURATION("Copy 1");
    for (int i = 0; i < 1000000; ++i) {
      editor2.Copy(1);
    }
  }

}

void StressTestLeftRight() {
  Editor editor;
  TypeText(editor, TEXT);

  {
    LOG_DURATION("Left");
    for (int i = 0; i < 1000000; ++i) {
      editor.Left();
    }
  }

  {
    LOG_DURATION("Right");
    for (int i = 0; i < 1000000; ++i) {
      editor.Right();
    }
  }

}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCopyPaste);
  RUN_TEST(tr, TestDefault);
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);

  StressTestInsertGet();
  StressTestCutPaste();
  StressTestCut();
  StressTestCopy();
  StressTestLeftRight();

  return 0;
}
