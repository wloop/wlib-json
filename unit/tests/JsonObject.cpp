#include <gtest/gtest.h>

#include <wlib/wjson/JsonObject.h>
#include <wlib/wjson/JsonElement.h>
#include <wlib/wjson/JsonNull.h>

using namespace wlp;

TEST(json_object, move_constructor) {
    json_object obj;

    json_element k1("key1");
    json_element k2("key2");
    json_element v1(24);
    json_element v2(75);

    obj.insert(k1, v1);
    obj.insert(k2, v2);

    ASSERT_EQ(2, obj.size());
    ASSERT_NE(obj.end(), obj.find(k1));
    ASSERT_NE(obj.end(), obj.find(k2));
    ASSERT_EQ(v1, *obj.find(k1));
    ASSERT_EQ(v2, *obj.find(k2));

    json_object moved(move(obj));

    ASSERT_EQ(0, obj.size());
    ASSERT_EQ(0, obj.capacity());
    ASSERT_EQ(obj.begin(), obj.end());

    ASSERT_EQ(2, moved.size());
    ASSERT_NE(moved.end(), moved.find(k1));
    ASSERT_NE(moved.end(), moved.find(k2));
    ASSERT_EQ(v1, *moved.find(k1));
    ASSERT_EQ(v2, *moved.find(k2));
}

TEST(json_object, move_operator) {
    json_element k1("key1");
    json_element k2("key2");
    json_element v1("val1");
    json_element v2("val2");

    json_object o1;
    json_object o2;

    o1.insert(k1, v1);
    o2.insert(k2, v2);

    ASSERT_EQ(1, o1.size());
    ASSERT_EQ(1, o2.size());

    ASSERT_NE(o1.end(), o1.find(k1));
    ASSERT_EQ(o1.end(), o1.find(k2));
    ASSERT_NE(o2.end(), o2.find(k2));
    ASSERT_EQ(o2.end(), o2.find(k1));

    o1 = move(o2);

    ASSERT_EQ(0, o2.size());
    ASSERT_EQ(0, o2.capacity());
    ASSERT_EQ(o2.begin(), o2.end());

    ASSERT_EQ(1, o1.size());
    ASSERT_EQ(o1.end(), o1.find(k1));
    ASSERT_NE(o1.end(), o1.find(k2));
}

TEST(json_object, null_keys) {
    json_object obj;

    json_element null_a;
    json_element null_b;

    json_element val1(10);
    json_element val2(20);

    obj.insert(null_a, val1);

    ASSERT_EQ(1, obj.size());
    ASSERT_TRUE(obj.contains(null_a));
    ASSERT_TRUE(obj.contains(null_b));

    auto ret = obj.insert(null_b, val2);

    ASSERT_FALSE(ret.second());
    ASSERT_EQ(*ret.first(), val1);
}

TEST(json_object, integer_keys) {
    json_element null_e;
    json_element bool_e(true);
    json_element int_e(-4923);
    json_element float_e(-2324.323f);
    json_element str_e("hello");

    json_element k1(25);
    json_element k2(35);
    json_element k3(45);
    json_element k4(55);

    json_object obj;

    obj.insert(k1, null_e);
    obj.insert(k2, bool_e);
    obj.insert(k3, int_e);
    obj.insert(k4, float_e);

    ASSERT_EQ(*obj.find(k1), null_e);
    ASSERT_EQ(*obj.find(k2), bool_e);
    ASSERT_EQ(*obj.find(k3), int_e);
    ASSERT_EQ(*obj.find(k4), float_e);

    ASSERT_TRUE(obj.erase(k1));
    ASSERT_TRUE(obj.erase(k2));

    auto ret = obj.insert(k3, str_e);
    ASSERT_FALSE(ret.second());
    ASSERT_EQ(*ret.first(), int_e);

    obj.insert(k1, str_e);
    obj.insert(k2, str_e);

    ASSERT_EQ(obj[k1], str_e);
    ASSERT_EQ(obj[k2], str_e);

    json_element k5(100);
    ASSERT_EQ(obj[k5], *json::null());
}

TEST(json_object, float_keys) {
    json_element str1("Their metal birds rained messages from the sky");
    json_element str2("They read: CITIZENS OF MITTENHEID,");
    json_element str3("THE LOSTLANDS CONFEDERACY HAS OBTAINED POWER OF THE GODS");
    json_element str4("SURRENDER OR FACE PROMPT AND UTTER DESTRUCTION");

    json_element k1(12.3);
    json_element k2(-23.12f);
    json_element k3(55.56);
    json_element k4(55.56);
    json_element k5(-232.23);

    json_object obj;

    obj.insert(k1, str1);
    obj.insert(k2, str2);
    obj.insert(k3, str3);
    obj.insert(k5, str4);

    ASSERT_EQ(4, obj.size());

    ASSERT_EQ(str1, *obj.find(k1));
    ASSERT_EQ(str2, *obj.find(k2));
    ASSERT_EQ(str3, *obj.find(k3));
    ASSERT_EQ(str4, *obj.find(k5));

    auto ret = obj.insert(k4, str1);
    ASSERT_FALSE(ret.second());
    ASSERT_EQ(*ret.first(), str3);

    ASSERT_TRUE(obj.erase(k1));
    ASSERT_TRUE(obj.erase(k2));
    ASSERT_TRUE(obj.erase(k3));
    ASSERT_FALSE(obj.erase(k4));
    ASSERT_TRUE(obj.erase(k5));

    ASSERT_EQ(0, obj.size());
}

TEST(json_object, string_keys) {
    json_element e1(55.6);
    json_element e2(false);
    json_element e3("TYPE_LONG_DOUBLE");

    json_element k1("key1");
    json_element k2("welcome");
    json_element k3("goodbye");

    json_object obj;

    obj.insert(k1, e1);
    obj.insert(k2, e2);
    obj.insert(k3, e3);

    ASSERT_EQ(3, obj.size());

    ASSERT_EQ(e1, *obj.find(k1));
    ASSERT_EQ(e2, *obj.find(k2));
    ASSERT_EQ(e3, *obj.find(k3));

    ASSERT_TRUE(obj.erase(k1));
    ASSERT_TRUE(obj.erase(k2));
    ASSERT_TRUE(obj.erase(k3));

    ASSERT_EQ(0, obj.size());

    obj.insert(k1, e1);
    obj.insert(k2, e2);

    ASSERT_EQ(2, obj.size());

    auto ret = obj.insert(k1, e3);
    ASSERT_FALSE(ret.second());
    ASSERT_EQ(e1, *ret.first());
}

TEST(json_object, move_insert) {
    constexpr auto keystr = "another string filled with numbers";
    constexpr auto valstr = "a string filled with characters";

    json_element val(valstr);
    json_element key(keystr);

    json_object obj;

    ASSERT_TRUE(obj.empty());

    obj.insert(move(key), move(val));

    ASSERT_EQ(*json::null(), key);
    ASSERT_EQ(*json::null(), val);

    ASSERT_EQ(1, obj.size());

    key = keystr;
    val = valstr;

    ASSERT_EQ(val, *obj.find(key));
}

TEST(json_object, implicit_insert) {
    json_object obj;

    obj.insert("key1", 111);
    obj.insert("key2", 222);
    obj.insert("key3", 333);

    ASSERT_EQ(3, obj.size());

    constexpr int sum = 111 + 222 + 333;
    int val = sum;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        val -= it->as<int>();
    }
    ASSERT_EQ(0, val);

    json_element key1("key1");
    json_element key2("key2");
    json_element key3("key3");

    ASSERT_NE(obj.end(), obj.find(key1));
    ASSERT_NE(obj.end(), obj.find(key2));
    ASSERT_NE(obj.end(), obj.find(key3));

    auto ret1 = obj.insert("key4", 444);
    ASSERT_TRUE(ret1.second());
    ASSERT_EQ(444, ret1.first()->as<int>());

    auto ret2 = obj.insert("key3", 666);
    ASSERT_FALSE(ret2.second());
    ASSERT_EQ(333, ret2.first()->as<int>());
}

TEST(json_object, implicit_insert_or_assign) {
    json_object obj;

    json_element e1(222);
    json_element e2(111);

    e2 = move(e1);

    ASSERT_EQ(e2.as<int>(), 222);

    auto ret1 = obj.insert_or_assign("key1", 111);
    ASSERT_EQ(1, obj.size());
    ASSERT_TRUE(ret1.second());
    ASSERT_EQ(111, ret1.first()->as<int>());

    auto it = obj.find("key1");
    ASSERT_NE(obj.end(), it);
    ASSERT_EQ(it->as<int>(), 111);

    auto ret2 = obj.insert("key1", 222);
    ASSERT_EQ(1, obj.size());
    ASSERT_FALSE(ret2.second());
    ASSERT_EQ(111, ret2.first()->as<int>());

    auto ret3 = obj.insert_or_assign("key1", 111);
    ASSERT_EQ(1, obj.size());
    ASSERT_FALSE(ret3.second());
    ASSERT_EQ(111, ret3.first()->as<int>());

    json_element key("hellokey");
    ASSERT_TRUE(obj.insert_or_assign("hellokey", "value").second());
    ASSERT_STREQ("value", obj[key].as<const char *>());

    ASSERT_FALSE(obj.insert_or_assign("hellokey", "anothervalue").second());
    ASSERT_STREQ("anothervalue", obj[key].as<const char *>());
}

TEST(json_object, implicit_erase) {
    json_object obj;
    obj.insert("first", "apple");
    obj.insert("second", "banana");
    obj.insert("third", "candy");

    obj.insert(5, 1234);
    obj.insert(nullptr, 0);

    ASSERT_EQ(5, obj.size());

    json_element key;
    ASSERT_EQ(0, obj[key].as<int>());

    key = 5;
    ASSERT_EQ(1234, obj[key].as<int>());

    key = "third";
    ASSERT_STREQ("candy", obj[key].as<const char *>());

    key = "second";
    ASSERT_STREQ("banana", obj[key].as<const char *>());

    key = "first";
    ASSERT_STREQ("apple", obj[key].as<const char *>());

    ASSERT_TRUE(obj.erase("first"));
    ASSERT_EQ(4, obj.size());
    ASSERT_EQ(obj.end(), obj.find(key));

    key = "second";
    ASSERT_TRUE(obj.erase("second"));
    ASSERT_EQ(3, obj.size());
    ASSERT_EQ(obj.end(), obj.find(key));

    key = "third";
    ASSERT_TRUE(obj.erase("third"));
    ASSERT_EQ(2, obj.size());
    ASSERT_EQ(obj.end(), obj.find(key));

    key = 5;
    ASSERT_TRUE(obj.erase(5));
    ASSERT_EQ(1, obj.size());
    ASSERT_EQ(obj.end(), obj.find(key));

    key = nullptr;
    ASSERT_TRUE(obj.erase(nullptr));
    ASSERT_EQ(0, obj.size());
    ASSERT_EQ(obj.end(), obj.find(key));
}

TEST(json_object, implicit_at) {
    json_object obj;
    obj.insert("first", 1);
    obj.insert("second", 2);
    obj.insert("third", 3);
    obj.insert("fourth", 4);
    obj.insert("fifth", 5);

    ASSERT_EQ(1, obj.at("first").as<int>());
    ASSERT_EQ(2, obj.at("second").as<int>());
    ASSERT_EQ(3, obj.at("third").as<int>());
    ASSERT_EQ(4, obj.at("fourth").as<int>());
    ASSERT_EQ(5, obj.at("fifth").as<int>());

    json_object cobj;
    cobj.insert("hello", 4);
    const json_object mobj = move(cobj);
    ASSERT_EQ(4, mobj.at("hello").as<int>());
}

TEST(json_object, implicit_contains) {
    json_object obj;
    obj.insert("hello", 4);
    ASSERT_TRUE(obj.contains("hello"));
    ASSERT_FALSE(obj.contains("goodbye"));
}

TEST(json_object, implicit_find) {
    json_object obj;
    obj.insert("hello", 1);
    obj.insert("goodbye", 5);
    ASSERT_NE(obj.end(), obj.find("hello"));
    ASSERT_EQ(obj.end(), obj.find("moshi"));
    const json_object cobj = move(obj);
    ASSERT_EQ(cobj.end(), cobj.find("who"));
    ASSERT_EQ(1, cobj.find("hello")->as<int>());
}

TEST(json_object, implicit_access) {
    json_object obj;
    obj.insert("one", 1);
    obj.insert("two", 2.2);
    obj.insert("three", 3.45f);

    ASSERT_EQ(1, obj["one"].as<int>());
    ASSERT_DOUBLE_EQ(2.2, obj["two"].as<double>());
    ASSERT_FLOAT_EQ(3.45f, obj["three"].as<float>());

    obj["three"] = 4;
    ASSERT_EQ(4, obj.find("three")->as<int>());

    obj["nine"] = "hello";
    ASSERT_STREQ("hello", obj.find("nine")->as<const char *>());
}

TEST(json_object, list_initialize) {
    json_object obj = {
        "first", 1,
        "second", 2,
        "third", 3
    };
    ASSERT_EQ(3, obj.size());
    ASSERT_EQ(1, obj["first"]);
    ASSERT_EQ(2, obj["second"]);
    ASSERT_EQ(3, obj["third"]);
}
