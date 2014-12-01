#ifndef __TAB_FUNCS_SORT_H
#define __TAB_FUNCS_SORT_H

template <typename T>
void sort_arratom(const obj::Object* in, obj::Object*& out) {

    obj::ArrayAtom<T>& x = obj::get< obj::ArrayAtom<T> >(in);
    std::sort(x.v.begin(), x.v.end());
    out = (obj::Object*)in;
}

void sort_arr(const obj::Object* in, obj::Object*& out) {

    obj::ArrayObject& x = obj::get<obj::ArrayObject>(in);
    std::sort(x.v.begin(), x.v.end(), obj::ObjectLess());
    out = (obj::Object*)in;
}

void sort_map(const obj::Object* in, obj::Object*& out) {

    obj::MapObject& a = obj::get<obj::MapObject>(in);
    obj::ArrayObject& o = obj::get<obj::ArrayObject>(out);
    
    typename obj::MapObject::map_t::const_iterator b = a.v.begin();
    typename obj::MapObject::map_t::const_iterator e = a.v.end();

    o.v.clear();
    
    while (b != e) {

        obj::Tuple* tmp = new obj::Tuple;
        tmp->v.resize(2);
        tmp->v[0] = b->first;
        tmp->v[1] = b->second;

        o.v.push_back(tmp);
        
        ++b;
    }

    std::sort(o.v.begin(), o.v.end(), obj::ObjectLess());
}

void sort_seq_arr(const obj::Object* in, obj::Object*& out) {

    out->fill((obj::Object*)in);
    sort_arr(out, out);
}

template <typename T>
void sort_seq_arratom(const obj::Object* in, obj::Object*& out) {

    out->fill((obj::Object*)in);
    sort_arratom<T>(out, out);
}

    
Functions::func_t sort_checker(const Type& args, Type& ret, obj::Object*& obj) {

    if (args.type == Type::ARR) {

        ret = args;
        obj = obj::nothing();

        const Type& t = args.tuple->at(0);

        if (t.type == Type::ATOM) {

            switch (t.atom) {
            case Type::INT:
                return sort_arratom<Int>;
            case Type::UINT:
                return sort_arratom<UInt>;
            case Type::REAL:
                return sort_arratom<Real>;
            case Type::STRING:
                return sort_arratom<std::string>;
            }

            return nullptr;

        } else {

            return sort_arr;
        }

    } else if (args.type == Type::MAP) {

        ret = Type(Type::ARR);

        Type pair(Type::TUP);
        pair.push(args.tuple->at(0));
        pair.push(args.tuple->at(1));

        ret.push(pair);

        return sort_map;

    } else if (args.type == Type::SEQ) {

        const Type& t = args.tuple->at(0);
        
        ret = Type(Type::ARR);
        ret.push(t);

        if (t.type == Type::ATOM) {

            switch (t.atom) {
            case Type::INT:
                return sort_seq_arratom<Int>;
            case Type::UINT:
                return sort_seq_arratom<UInt>;
            case Type::REAL:
                return sort_seq_arratom<Real>;
            case Type::STRING:
                return sort_seq_arratom<std::string>;
            }

            return nullptr;
            
        } else {
            return sort_seq_arr;
        }
    }
        
    return nullptr;
}

void register_sort(Functions& funcs) {

    funcs.add_poly("sort", sort_checker);
}

#endif
