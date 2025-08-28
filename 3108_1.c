static int loco_get_rice_param(RICEContext *r) {
    int cnt = 0;
    int val = r->count;
    while (r->sum > val && cnt < 9) {
        val <<= 1;
        cnt++;
    }
    return cnt;
}

void example_usage(RICEContext *r) {
    int param = loco_get_rice_param(r);
    if(param > 8) {
    }
}