#ifndef HASHTABLE_H
#define HASHTABLE_H

#pragma once

#include "common.h"
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace gedis {

struct HNode {
    HNode *next {nullptr};
    uint64_t hcode{0};
    HNode() = delete;
    HNode(uint64_t h) : next{nullptr}, hcode{h} {}
};

const size_t  k_resizing_work = 128;    // 扩容时一次处理的节点数量
const size_t k_max_load_factor = 8;     // 最大负载系数

using Cmp = std::function<bool(HNode *, HNode *)>;
using NodeScan = std::function<void(HNode *, void *)>;
using Nodedispose = std::function<void(HNode *)>;

class HMap;

class HTab {
  private:
    std::vector<HNode *> tab{nullptr};  // 使用指针数组来存储哈希桶，每个桶存储一个链表
    size_t mask{0}, size{0};
    // mask 用于确定索引范围，值为 n-1，其中 n 是桶的数量，且 n 必须是 2 的幂。
    // size 用于跟踪哈希表中元素的数量

    friend HMap;

    HTab() = default;
    HTab(size_t n) : tab{n}, mask{n-1}, size{0} {
        std::cout << "!!!HTab -> n = " << n << "\n";
        assert(n > 0 && ((n - 1) & n) == 0);
    }

    // 头插法加入到 tab 指针链表
    void insert(HNode *node) {
        size_t const pos = node->hcode & mask;
        HNode *next = tab[pos];
        node->next = next;
        tab[pos] = node;
        size ++;
    }
    HNode **lookup(HNode *key, const Cmp& cmp) {
        if(tab.empty()) { return nullptr; }
        size_t const pos = key->hcode & mask;
        HNode **from = &tab[pos];
        (void)*from;

        while(*from) {
            if(cmp(*from, key)) { return from; }
            from = &(*from)->next;
        }
        return nullptr;
    }
    HNode *detach(HNode **from) {
        HNode *node = *from;
        *from = (*from)->next;
        size --;
        return node;
    }
    void scan(const NodeScan& node_scan, void *extra) {
        if(size == 0) { return; }
        for(size_t i = 0; i < mask + 1; i ++) {
            HNode *node = tab[i];
            while(node) {
                node_scan(node, extra);
                node = node->next;
            }
        }
    }
    void dispose(const Nodedispose& node_dispose) {
        if(size == 0) { return; }
        for(size_t i = 0; i < mask + 1; i ++) {
            HNode *node = tab[i];
            while(node) {
                HNode *next = node->next;
                node_dispose(node);
                node = next;
            }
        }
    }
};

class HMap {
  private:
    HTab ht1{}, ht2{};
    size_t resizing_pos{0};

  public:
    HMap() = default;
    [[nodiscard]] size_t  size() const { return ht1.size + ht2.size; }
    HNode *lookup(HNode *key, const Cmp& cmp) {
        help_resizing();
        HNode **from = nullptr;
        from = ht1.lookup(key, cmp);
        if(!from) { from = ht2.lookup(key, cmp); }
        return from ? *from : nullptr;
    }
    // 用于帮助哈希表进行扩容。当第二个 HTab 中有节点时，它会将节点
    // 逐个转移到第一个 HTab 中，直到完成指定数量的工作或者第二个 HTab 中没有节点为止。
    void help_resizing() {
        if(ht2.tab.empty()) { return; }
        size_t nwork = 0;
        while(nwork < k_resizing_work && ht2.size > 0) {
            HNode **from = &ht2.tab[resizing_pos];
            if(!*from) {
                resizing_pos ++;
                continue;
            }

            ht1.insert(ht2.detach(from));
            nwork++;
        }
        if(ht2.size == 0) { ht2 = std::move(HTab{}); }
    }
    void insert(HNode *node) {
        if(ht1.tab.empty()) { ht1 = HTab{4}; }
        ht1.insert(node);
        if(ht2.tab.empty()) {
            size_t const load_factor = ht1.size / (ht1.mask + 1);
            if(load_factor >= k_max_load_factor) { start_resizing(); }
        }
        help_resizing();
    }
    void start_resizing() {
        assert(ht2.tab.empty());
        ht2 = std::move(ht1);
        ht1 = std::move(HTab{4});
        resizing_pos = 0;
    }
    HNode *pop(HNode *key, const Cmp& cmp) {
        help_resizing();
        HNode **from = ht1.lookup(key, cmp);
        if(from) { return ht1.detach(from); }
        from = ht2.lookup(key, cmp);
        if(from) { return ht2.detach(from); }
        return nullptr;
    }
    void scan(const NodeScan& node_scan, void *extra) {
        ht1.scan(node_scan, extra);
        ht2.scan(node_scan, extra);
    }
    void dispos(const Nodedispose& node_dispose) {
        ht1.dispose(node_dispose);
        ht2.dispose(node_dispose);
    }
};

inline uint64_t string_hash(const std::string &str) {
    uint32_t h = 0x811C9DC5;
    for (auto c : str) { h = (h + (uint8_t)c) * 0x01000193; }
    return h;
}

} // namespace gedis

#endif
