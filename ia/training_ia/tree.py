import sys
from typing import Tuple, List
from sklearn import tree
from sklearn.datasets import load_iris
import matplotlib.pyplot as plt


def make_tree_classifier() -> None:
    iris = load_iris()
    X, y = iris.data, iris.target
    clf = tree.DecisionTreeClassifier()
    clf = clf.fit(X, y)
    tree.plot_tree(clf)
    print(tree.export_graphviz(clf, out_file=None, feature_names=iris.feature_names))