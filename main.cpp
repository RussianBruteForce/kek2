#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <vector>
#include <locale>
#include <functional>
#include <map>
#include <memory>
#include "bts.h"

using namespace std;

constexpr auto wordToFind="нашел";

#ifndef QT_NO_DEBUG
template<typename T>
class TD;
#endif

// Слово и кол-во упоминаний слова
struct BtsData {
	using int_type = std::uint_fast32_t;
	BtsData() = default;
	BtsData(QString&& w, int_type c = 1):
	        word(std::move(w)),
	        count(c){
		qDebug() << "by rv" << word;
	}
	BtsData(QString& w, int_type c = 1):
	        word(w),
	        count(c){}
	QString word;
	int_type count, depth{0};
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
	hash<string> str_hash; // функция хеширования слова
	map<size_t, unique_ptr<BtsData>> m; // мап с ключем-хешем

	// добавление в мап с инкрементацией
	// счетчика, если уже добавлено
	auto add_to_map = [&m, &str_hash] (QString &s) {
		auto h = str_hash(s.toStdString());
		auto x = m.find(h);
		if (x != m.cend())
			x->second->count++;
		else
			m.emplace(h, make_unique<BtsData>(s));
	};

	auto trim = [] (QString&& s) {
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
			                )
				return QString();
			else if (x.category() != QChar::Letter_Lowercase)
				x = ' ';
		}
		//TD<decltype(s)> sd;
		return s.trimmed();
	};

	auto start = QTime::currentTime();
	qDebug() << "Opening file..." << start;
	QFile textfile{"w2"};
	textfile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream text{&textfile};

	QString word;

	qDebug() << "Loading file to map..." <<  QTime::currentTime().msecsTo(start);
	while (!text.atEnd()) {
		text >> word;
		word = trim(word.toLower());

		if (word.size() == 0)
			continue;
		add_to_map(word);
	}
	textfile.close();

	qDebug() << "To BTS" <<  QTime::currentTime().msecsTo(start);
	for (auto&x: m) {
		bts.add(*x.second);
	}

	auto fTime = QTime::currentTime().msecsTo(start);
	m.find(str_hash(wordToFind));
	qDebug() << "MAP FIND TIME ~" <<  QTime::currentTime().msecsTo(start) - fTime;

	auto find = [&bts, &start] (QString s) {
		auto fTime = QTime::currentTime().msecsTo(start);
		bts.traverse([&s, &start, &fTime](BtsData d) -> bool {
			if (d.word == s) {
				qDebug() << "TREE FIND TIME ~" << QTime::currentTime().msecsTo(start) - fTime;
				qDebug() << d.word << d.count << d.depth;
				return true;
			}
			return false;
		});
	};

	find(wordToFind);
	qDebug() << "FINISH" <<  QTime::currentTime().msecsTo(start);
	bts.add({"sdf", 3000});

	return 0;
}

