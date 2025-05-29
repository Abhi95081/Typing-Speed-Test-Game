#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isWord;

    TrieNode() : isWord(false) {}
};

class Trie {
private:
    TrieNode* root;
    vector<string> words;

    void freeTrie(TrieNode* node) {
        for (auto& [ch, child] : node->children)
            freeTrie(child);
        delete node;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (!node->children.count(ch))
                node->children[ch] = new TrieNode();
            node = node->children[ch];
        }
        node->isWord = true;
        words.push_back(word);
    }

    string getRandomWord() {
        if (words.empty()) return "";
        int index = rand() % words.size();
        return words[index];
    }

    ~Trie() {
        freeTrie(root);
    }
};

int countMistakes(const string& original, const string& typed, int& totalWordsTyped) {
    stringstream os(original), ts(typed);
    string ow, tw;
    int mistakes = 0;
    totalWordsTyped = 0;

    while (ts >> tw) {
        totalWordsTyped++;
        if (!(os >> ow) || tw != ow)
            mistakes++;
    }

    // If any original words are left unmatched
    while (os >> ow)
        mistakes++;

    return mistakes;
}

int calculateWPM(int wordCount, double elapsedTime) {
    return static_cast<int>(wordCount / (elapsedTime / 60.0));
}

void runTypingTest(Trie& trie) {
    unordered_set<string> used;
    string testText;

    while (used.size() < 20) {
        string word = trie.getRandomWord();
        if (!used.count(word)) {
            used.insert(word);
            testText += word + " ";
        }
    }

    cout << "\n Type the following text:\n\n";
    cout << testText << "\n";
    cout << "\n Press Enter when ready...";
    cin.ignore();

    // Start timer
    clock_t start = clock();
    cout << "\n Start typing:\n";
    string typedText;
    getline(cin, typedText);
    clock_t end = clock();

    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    int totalWordsTyped = 0;
    int mistakes = countMistakes(testText, typedText, totalWordsTyped);
    int wpm = calculateWPM(totalWordsTyped, elapsed);
    int totalWords = count(testText.begin(), testText.end(), ' ');
    float accuracy = totalWords ? ((totalWords - mistakes) / (float)totalWords) * 100.0f : 0;

    // Show Results
    cout << fixed << setprecision(2);
    cout << "\n Time Taken: " << elapsed << " seconds";
    cout << "\n Words Per Minute (WPM): " << wpm;
    cout << "\n Mistakes: " << mistakes;
    cout << "\n Accuracy: " << accuracy << "%\n";
}

int main() {
    Trie trie;
   srand(time(0));

    // Load words into trie
    ifstream file("words.txt");
    if (!file) {
        cerr << "❌ Failed to load words from words.txt\n";
        return 1;
    }
    string word;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        trie.insert(word);
    }

    char choice;
    do {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        runTypingTest(trie);
        cout << "\n🔁 Try again? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "\n👋 Thank you for playing. Goodbye!\n";
    return 0;
}
