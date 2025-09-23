package algorithms;

import java.awt.Point;
import java.util.*;

public class DefaultTeam {

    public ArrayList<Point> calculFVS(ArrayList<Point> points, int edgeThreshold) {
        // Utiliser un algorithme glouton amélioré pour générer une solution initiale
        ArrayList<Point> initialSolution = improvedGreedySolution(points, edgeThreshold);
        // Exécuter l'algorithme de recuit simulé optimisé
        ArrayList<Point> optimizedSolution = simulatedAnnealing(initialSolution, points, edgeThreshold);
        return optimizedSolution;
    }

    // Algorithme glouton amélioré
    private ArrayList<Point> improvedGreedySolution(ArrayList<Point> points, int edgeThreshold) {
        // Construire la liste d'adjacence du graphe
        Map<Point, Set<Point>> adjacencyList = buildGraph(points, edgeThreshold);

        // Initialiser l'ensemble des sommets de feedback (FVS) et les sommets du graphe
        Set<Point> fvs = new HashSet<>();
        Set<Point> vertices = new HashSet<>(points);

        // Boucle principale
        while (!vertices.isEmpty()) {
            // Supprimer les sommets avec un degré 0 ou 1 (feuilles)
            List<Point> leaves = new ArrayList<>();
            Set<Point> verticesCopy = new HashSet<>(vertices);
            for (Point v : verticesCopy) {
                if (adjacencyList.get(v).size() <= 1) {
                    leaves.add(v);
                }
            }
            while (!leaves.isEmpty()) {
                Point leaf = leaves.remove(leaves.size() - 1);
                if (!vertices.contains(leaf)) continue;
                vertices.remove(leaf);
                for (Point neighbor : new HashSet<>(adjacencyList.get(leaf))) {
                    adjacencyList.get(neighbor).remove(leaf);
                    if (adjacencyList.get(neighbor).size() <= 1) {
                        leaves.add(neighbor);
                    }
                }
                adjacencyList.get(leaf).clear();
            }

            // Si tous les sommets sont supprimés, le graphe est acyclique
            if (vertices.isEmpty()) {
                break;
            }

            // Sinon, choisir le sommet avec le meilleur score (degré et triangles) pour le retirer
            Point vertexToRemove = selectVertexToRemove(vertices, adjacencyList);
            vertices.remove(vertexToRemove);
            fvs.add(vertexToRemove);

            // Mettre à jour la liste d'adjacence
            for (Point neighbor : new HashSet<>(adjacencyList.get(vertexToRemove))) {
                adjacencyList.get(neighbor).remove(vertexToRemove);
            }
            adjacencyList.get(vertexToRemove).clear();
        }

        return new ArrayList<>(fvs);
    }

    // Choisir le sommet à retirer en fonction du degré et du nombre de triangles
    private Point selectVertexToRemove(Set<Point> vertices, Map<Point, Set<Point>> adjacencyList) {
        Point selectedVertex = null;
        double maxScore = Double.NEGATIVE_INFINITY;

        for (Point v : vertices) {
            int degree = adjacencyList.get(v).size();
            int triangleCount = countTriangles(v, adjacencyList);
            double score = degree + triangleCount * 2; // Pondération ajustable

            if (score > maxScore) {
                maxScore = score;
                selectedVertex = v;
            }
        }

        return selectedVertex;
    }

    // Calculer le nombre de triangles impliquant un sommet
    private int countTriangles(Point v, Map<Point, Set<Point>> adjacencyList) {
        int count = 0;
        Set<Point> neighbors = adjacencyList.get(v);
        for (Point u : neighbors) {
            for (Point w : new HashSet<>(adjacencyList.get(u))) {
                if (adjacencyList.get(v).contains(w) && !w.equals(v) && !w.equals(u)) {
                    count++;
                }
            }
        }
        // Chaque triangle est compté trois fois, il faut diviser par 3
        return count / 3;
    }

    // Construire la liste d'adjacence du graphe
    private Map<Point, Set<Point>> buildGraph(ArrayList<Point> points, int edgeThreshold) {
        Map<Point, Set<Point>> graph = new HashMap<>();
        for (Point p : points) {
            graph.put(p, new HashSet<>());
        }
        for (int i = 0; i < points.size(); i++) {
            Point p = points.get(i);
            for (int j = i + 1; j < points.size(); j++) {
                Point q = points.get(j);
                if (p.distance(q) < edgeThreshold) {
                    graph.get(p).add(q);
                    graph.get(q).add(p);
                }
            }
        }
        return graph;
    }

    // Algorithme de recuit simulé
    private ArrayList<Point> simulatedAnnealing(ArrayList<Point> currentSolution, ArrayList<Point> points, int edgeThreshold) {
        double temperature = 1000.0;
        double coolingRate = 0.999;
        int maxIterations = 2000000; // Ajuster le nombre maximal d'itérations

        ArrayList<Point> bestSolution = new ArrayList<>(currentSolution);
        int bestCost = bestSolution.size();

        Random random = new Random();

        for (int iteration = 0; iteration < maxIterations; iteration++) {
            ArrayList<Point> newSolution = generateNeighbor(currentSolution, points, random);

            if (isValidSolution(newSolution, points, edgeThreshold)) {
                int currentCost = currentSolution.size();
                int newCost = newSolution.size();

                if (newCost < currentCost) {
                    currentSolution = new ArrayList<>(newSolution);
                    if (newCost < bestCost) {
                        bestSolution = new ArrayList<>(newSolution);
                        bestCost = newCost;
                    }
                } else {
                    double acceptanceProbability = Math.exp((currentCost - newCost) / temperature);
                    if (random.nextDouble() < acceptanceProbability) {
                        currentSolution = new ArrayList<>(newSolution);
                    }
                }
            }

            // Réduction de la température
            temperature *= coolingRate;
            if (temperature < 1e-256) {
                break;
            }
        }

        // Dernière optimisation par recherche locale
        bestSolution = localSearch(bestSolution, points, edgeThreshold);

        return bestSolution;
    }

    // Générer une solution voisine
    private ArrayList<Point> generateNeighbor(ArrayList<Point> solution, ArrayList<Point> points, Random random) {
        ArrayList<Point> newSolution = new ArrayList<>(solution);
        int operation = random.nextInt(3);

        if (operation == 0 && !solution.isEmpty()) {
            // Supprimer un sommet aléatoire
            int indexToRemove = random.nextInt(solution.size());
            newSolution.remove(indexToRemove);
        } else if (operation == 1) {
            // Ajouter un sommet aléatoire
            ArrayList<Point> candidates = new ArrayList<>(points);
            candidates.removeAll(solution);
            if (!candidates.isEmpty()) {
                int indexToAdd = random.nextInt(candidates.size());
                newSolution.add(candidates.get(indexToAdd));
            }
        } else if (operation == 2 && !solution.isEmpty()) {
            // Remplacer un sommet aléatoire
            int indexToReplace = random.nextInt(solution.size());
            Point pointToRemove = solution.get(indexToReplace);
            ArrayList<Point> candidates = new ArrayList<>(points);
            candidates.removeAll(solution);
            if (!candidates.isEmpty()) {
                int indexToAdd = random.nextInt(candidates.size());
                Point pointToAdd = candidates.get(indexToAdd);
                newSolution.set(indexToReplace, pointToAdd);
            } else {
                newSolution.remove(pointToRemove);
            }
        }

        return newSolution;
    }

    // Vérifier la validité de la solution
    private boolean isValidSolution(ArrayList<Point> candidateSolution, ArrayList<Point> points, int edgeThreshold) {
        Map<Point, Set<Point>> graph = buildGraph(points, edgeThreshold);
        for (Point v : candidateSolution) {
            removeVertex(graph, v);
        }
        return !hasCycle(graph);
    }

    // Supprimer un sommet du graphe
    private void removeVertex(Map<Point, Set<Point>> graph, Point vertex) {
        if (graph.containsKey(vertex)) {
            Set<Point> neighbors = new HashSet<>(graph.get(vertex));
            for (Point neighbor : neighbors) {
                graph.get(neighbor).remove(vertex);
            }
            graph.remove(vertex);
        }
    }

    // Vérifier s'il y a un cycle dans le graphe
    private boolean hasCycle(Map<Point, Set<Point>> graph) {
        Set<Point> visited = new HashSet<>();
        for (Point node : graph.keySet()) {
            if (!visited.contains(node)) {
                if (dfsHasCycle(graph, node, null, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    private boolean dfsHasCycle(Map<Point, Set<Point>> graph, Point current, Point parent, Set<Point> visited) {
        visited.add(current);
        for (Point neighbor : graph.get(current)) {
            if (!neighbor.equals(parent)) {
                if (visited.contains(neighbor)) {
                    return true;
                } else {
                    if (dfsHasCycle(graph, neighbor, current, visited)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // Recherche locale pour optimiser la solution
    private ArrayList<Point> localSearch(ArrayList<Point> solution, ArrayList<Point> points, int edgeThreshold) {
        ArrayList<Point> improvedSolution = new ArrayList<>(solution);
        boolean improved = true;

        while (improved) {
            improved = false;
            for (int i = 0; i < improvedSolution.size(); i++) {
                Point removedPoint = improvedSolution.remove(i);
                if (isValidSolution(improvedSolution, points, edgeThreshold)) {
                    improved = true;
                    break;
                } else {
                    improvedSolution.add(i, removedPoint);
                }
            }
        }

        return improvedSolution;
    }
}
