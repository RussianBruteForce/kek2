#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <locale>
#include <functional>
#include <map>
#include "bts.h"

using namespace std;

// Слово и кол-во упоминаний слова
struct BtsData {
	QString word;
	int count, depth;
	// операция сравнения по кол-ву включений в текст
	friend inline bool operator< (const BtsData& lhs, const BtsData& rhs){
		return ( lhs.count < rhs.count) ? true: false;
	}
	// делаем слово "глубже"
	void deep() {depth++;}
};

int main()
{
	Bts<BtsData> bts; // дерево поиска
	std::hash<string> str_hash; // функция хеширования слова
	map<size_t, BtsData*> m; // мап с ключем-хешем

	// добавление в мап с инкрементацией
	// счетчика, если уже добавлено
	auto add_to_map = [&m, &str_hash] (QString &s) {
		auto h = str_hash(s.toStdString());
		auto x = m.find(h);
		if (x != m.end())
			x->second->count++;
		else {
			auto d = new BtsData({s, 1, 0});
			m.emplace(h, d);
		}
	};

	auto trim = [] (QString s) {
		for (auto &x: s) {
			if (
			                x == QChar('a') ||
			                x == QChar('b') ||
			                x == QChar('c') ||
			                x == QChar('d') ||
			                x == QChar('e') ||
			                x == QChar('f') ||
			                x == QChar('g') ||
			                x == QChar('h') ||
			                x == QChar('i') ||
			                x == QChar('j') ||
			                x == QChar('k') ||
			                x == QChar('l') ||
			                x == QChar('m') ||
			                x == QChar('n') ||
			                x == QChar('p') ||
			                x == QChar('p') ||
			                x == QChar('q') ||
			                x == QChar('r') ||
			                x == QChar('s') ||
			                x == QChar('t') ||
			                x == QChar('u') ||
			                x == QChar('v') ||
			                x == QChar('w') ||
			                x == QChar('x') ||
			                x == QChar('y') ||
			                x == QChar('z')
			                ) {
				s.clear();
				return QString();
			} else if (x.category() != QChar::Letter_Lowercase)
				x = ' ';

		}
		return s.trimmed();
	};

	QFile textfile("w2");
	textfile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream text(&textfile);

	QString word;
	do {
		text >> word;
		word = trim(word.toLower());

		if (word.size() == 0)
			continue;
		add_to_map(word);
	} while (!text.atEnd());
	textfile.close();

	for (auto& x: m) {
//		qDebug() << " [" << x.first <<
//		        ':' << x.second->word <<
//		        ", " << x.second->count << ']';
		BtsData d = *x.second;
		bts.add(d);
	}

	bts.traverse([](BtsData d){
		qDebug() << d.word << d.count << d.depth;
	});

	auto find = [&bts] (QString s) {
		bts.traverse([&s](BtsData d){
			if (d.word == s)
				qDebug() << d.word << d.count << d.depth;
		});
	};

	find("мама");

	return 0;
}
