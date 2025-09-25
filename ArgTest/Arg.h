#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class ArgItem {
private:
    int _valueCoount;
    std::vector<std::string> _opts;
    std::vector<std::string> _values;
    bool _isSet;
    std::string toLowwer(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return s;
    }
public:
    ArgItem(std::vector<std::string> ops, int vc)
    {
        _opts.clear();
        _values.clear();
        for (const std::string k : ops)
        {
            _opts.push_back(toLowwer(k));

        }
        _valueCoount = vc;
        _isSet = false;
    }
    int valueCoount() const { return _valueCoount; };
    void setValues(std::vector<std::string> val)
    {
        _values.clear();
        if (val.size() == 0 || _valueCoount == 0) {
            return;
        }
        if (_values.size() <= _valueCoount) {
            for (const std::string& s : val)
            {
                _values.push_back(s);
            }
        }
    }
    std::vector<std::string> opts() const { return _opts; };
    std::vector<std::string> values() const { return _values; }
	void setIsSet(bool b) { _isSet = b; }
	bool isSet() const { return _isSet; }
};

class Arg {
private:
	std::string _exePath;
    std::vector<std::string> _items;
    std::vector<ArgItem> _args;
    std::vector<std::string> _argItem;
	bool _isOpErr = false;
	int _optCount = 0;
    bool isOpt(std::string s)
    {
        return(s != "" && (s[0] == '/' || s[0] == '-'));
    }
    std::string toLowwer(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return s;
    }
public:
	std::string exePath() const { return _exePath; }
	std::vector<std::string> items() const { return _items; }
    std::string items(int idx) const {
        if (idx < 0 || idx >= (int)_items.size()) {
            return "";
		}
        return _items[idx]; 
    }
    std::vector<ArgItem> args()const { return _args; }
    std::vector<std::string> argItem() const { return _argItem; }
    int optCount() const { return _optCount; }
    // コンストラクタ（オブジェクト生成時に呼ばれる）
    Arg(int argc, char* argv[]) 
    {
		_isOpErr = false;
        _optCount = 0;
        if (argc > 0) {
            _exePath = std::string(argv[0]);
		}
        if (argc > 1) {
            for (int i = 1; i < argc; ++i) {
                _items.push_back(std::string(argv[i]));
            }
        }
    }

    void addArgItem(ArgItem item) {
        if (item.opts().size() > 0) {
            _args.push_back(item);
        }
	}
    void addArgItems(std::vector<ArgItem> items) {
        for (const ArgItem& s : items)
        {
            addArgItem(s);
        }
    }
    void parse()
    {
        _argItem.clear();
        if (_args.size() <= 0)
        {
            for (const std::string& s : _items)
            {
                _argItem.push_back(s);
            }
            return;
        }
        for (int i = 0; i < _items.size(); i++)
        {
            if (isOpt(_items[i]))
            {
                std::string op = toLowwer(_items[i].substr(1));
                for (int ii=0; ii<_args.size();ii++)
                {
					if (_args[ii].isSet()) continue;
					bool isOPT = false;
                    for (const std::string k : _args[ii].opts())
                    {
                        if (k == op)
                        {
                            isOPT = true;
                            break;
						}
                    }
                    if (!isOPT) 
                    {
						_isOpErr = true;
                    }
                    else
                    {
                        if (_args[ii].valueCoount() > 0) {
                            if (i < _items.size() - 1) 
                            {
                                std::vector<std::string> vals;
                                for (int j = 0 ; j < _args[ii].valueCoount(); j++)
                                {
                                    i++;
                                    if (i >= _items.size()) {
										i--;
                                        break;
                                    }
                                    if (isOpt(_items[i])) {
                                        i--;
                                        break;
									}
                                    vals.push_back(_items[i]);
                                }
                                _args[ii].setValues(vals);
                            }
                        }
						_args[ii].setIsSet(true);
                        _optCount++;
                    }
                }

            }
            else {
                _argItem.push_back(_items[i]);
            }
        }
    }
};
