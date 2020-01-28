void inordine(Nod* n) {
    if (nrFii > 0) inordine(n->adrFii[0]);
    cout << n->info << " ";
    for (int i = 1; i < n->nrFii; ++i) {
        inordine(n->adrFii[i]);
    }
}
