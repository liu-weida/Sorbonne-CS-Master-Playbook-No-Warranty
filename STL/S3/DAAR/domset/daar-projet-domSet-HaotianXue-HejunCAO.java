package algorithms;

import java.awt.Point;
import java.util.*;
import java.util.stream.Collectors;

public class DefaultTeam {
  public ArrayList<Point> calculDominatingSet(ArrayList<Point> points, int edgeThreshold) {
    DominatingSet dominatingSet = new DominatingSet(points, edgeThreshold);
    return dominatingSet.calculateSolution();
  }
}

class DominatingSet {
  private final ArrayList<Point> points;
  private final int edgeThreshold;
  private List<List<Integer>> adjList;
  private Map<Point,Integer> pointIndex;

  // 一些参数可调优
  private static final double PAIR_DISTANCE_FACTOR = 2.5;
  private static final double TRIPLE_DISTANCE_FACTOR = 3.0;
  private static final int CANDIDATE_LIMIT = 50;
  private static final int PAIR_LIMIT = 50;
  private static final int TRIPLE_LIMIT = 50; 

  public DominatingSet(ArrayList<Point> points, int edgeThreshold) {
    this.points = points;
    this.edgeThreshold = edgeThreshold;
    buildAdjList();
  }

  private void buildAdjList() {
    int n = points.size();
    pointIndex = new HashMap<>(n);
    for (int i = 0; i < n; i++) {
      pointIndex.put(points.get(i), i);
    }

    adjList = new ArrayList<>(n);
    for (int i = 0; i < n; i++) adjList.add(new ArrayList<>());
    for (int i = 0; i < n; i++) {
      Point p = points.get(i);
      for (int j = i+1; j < n; j++) {
        Point q = points.get(j);
        if (p.distance(q) < edgeThreshold) {
          adjList.get(i).add(j);
          adjList.get(j).add(i);
        }
      }
    }
  }

  public ArrayList<Point> calculateSolution() {
    System.out.println("Calculating improved initial solution...");
    ArrayList<Point> initialSolution = buildHighQualityInitialSolution();

    System.out.println("Local improvement using remove2Add1 & remove3Add2...");
    ArrayList<Point> improved = localImprovement(initialSolution);

    System.out.println("Optimizing with Simulated Annealing...");
    ArrayList<Point> finalSolution = simulatedAnnealing(improved);

    System.out.println("Final MDS size: " + finalSolution.size());
    return finalSolution;
  }

  private ArrayList<Point> buildHighQualityInitialSolution() {
    ArrayList<Point> sortedPoints = new ArrayList<>(points);
    sortedPoints.sort(Comparator.comparingDouble(Point::getX).thenComparingDouble(Point::getY));

    int n = points.size();
    boolean[] dominated = new boolean[n];
    ArrayList<Point> solution = new ArrayList<>();

    for (int i = 0; i < n; i++) {
      int idx = indexOf(sortedPoints.get(i));
      if (!dominated[idx]) {
        int bestCandidate = -1;
        int bestCoverage = -1;
        for (int j = i; j < n; j++) {
          int cIdx = indexOf(sortedPoints.get(j));
          if (canCover(cIdx, idx)) {
            int coverage = countCoverage(cIdx, dominated);
            if (coverage > bestCoverage) {
              bestCoverage = coverage;
              bestCandidate = cIdx;
            }
          }
        }
        if (bestCandidate == -1) {
          bestCandidate = idx;
        }
        solution.add(points.get(bestCandidate));
        markDominated(bestCandidate, dominated);
      }
    }

    solution = cleanUselessDominatingPoints(solution);
    return solution;
  }

  private boolean canCover(int candidate, int v) {
    if (candidate == v) return true;
    for (int nei : adjList.get(candidate)) {
      if (nei == v) return true;
    }
    return false;
  }

  private int indexOf(Point p) {
    return pointIndex.get(p);
  }

  private int countCoverage(int candidate, boolean[] dominated) {
    int count = 0;
    if (!dominated[candidate]) count++;
    for (int nei : adjList.get(candidate)) {
      if (!dominated[nei]) count++;
    }
    return count;
  }

  private void markDominated(int idx, boolean[] dominated) {
    dominated[idx] = true;
    for (int nei : adjList.get(idx)) {
      dominated[nei] = true;
    }
  }

  private ArrayList<Point> localImprovement(ArrayList<Point> currentSol) {
    ArrayList<Point> solution = new ArrayList<>(currentSol);
    ArrayList<Point> improved;

    do {
      improved = heuristicRemove2Add1(solution);
      if (improved.size() < solution.size()) {
        solution = improved;
      }
    } while (improved.size() < solution.size());

    do {
      improved = heuristicRemove3Add2(solution);
      if (improved.size() < solution.size()) {
        solution = improved;
      }
    } while (improved.size() < solution.size());

    return solution;
  }

  /**
   * - 预先选择潜在的有前景的点对（相距不超过一定倍数的edgeThreshold，且在解中）
   * - 对点对进行潜在收益排序（如根据他们覆盖区域的重叠度、是否可以用少数候选点覆盖）
   * - 从中选出前PAIR_LIMIT个点对尝试
   * - 候选添加点仅从高覆盖力的前CANDIDATE_LIMIT个点中挑选
   */
  private ArrayList<Point> heuristicRemove2Add1(ArrayList<Point> solution) {
    int n = points.size();
    boolean[] dominated = computeDominated(solution);
    Set<Integer> inSol = solution.stream().map(this::indexOf).collect(Collectors.toSet());

    List<int[]> candidatePairs = new ArrayList<>();
    List<Integer> solIndices = new ArrayList<>(inSol);
    for (int i = 0; i < solIndices.size(); i++) {
      int A = solIndices.get(i);
      for (int j = i+1; j < solIndices.size(); j++) {
        int B = solIndices.get(j);
        double dist = points.get(A).distance(points.get(B));
        if (dist <= PAIR_DISTANCE_FACTOR * edgeThreshold) {
          candidatePairs.add(new int[]{A,B});
        }
      }
    }


    candidatePairs.sort(Comparator.comparingDouble(pair -> points.get(pair[0]).distance(points.get(pair[1]))));

    if (candidatePairs.size() > PAIR_LIMIT) {
      candidatePairs = candidatePairs.subList(0, PAIR_LIMIT);
    }

    List<Integer> candidateAddPoints = selectTopCandidatePoints(dominated, inSol, CANDIDATE_LIMIT);


    ArrayList<Point> best = new ArrayList<>(solution);
    int bestSize = best.size();
    for (int[] pair : candidatePairs) {
      int A = pair[0];
      int B = pair[1];

      for (int C : candidateAddPoints) {
        ArrayList<Point> newSol = new ArrayList<>(solution);
        newSol.remove(points.get(A));
        newSol.remove(points.get(B));
        newSol.add(points.get(C));
        if (newSol.size() < bestSize && isSolutionValid(newSol)) {
          return cleanUselessDominatingPoints(newSol);
        }
      }
    }
    return best;
  }

  /**
   * - 仅考虑距离较近的三元组（相互距离不大）
   * - 仅考虑前TRIPLE_LIMIT个最有前景的三元组
   * - 候选点从前CANDIDATE_LIMIT个添加点中选
   */
  private ArrayList<Point> heuristicRemove3Add2(ArrayList<Point> solution) {
    int n = points.size();
    boolean[] dominated = computeDominated(solution);
    Set<Integer> inSol = solution.stream().map(this::indexOf).collect(Collectors.toSet());

    List<Integer> solIndices = new ArrayList<>(inSol);

    List<int[]> candidateTriples = new ArrayList<>();

    for (int i = 0; i < solIndices.size(); i++) {
      int A = solIndices.get(i);
      for (int j = i+1; j < solIndices.size(); j++) {
        int B = solIndices.get(j);
        double distAB = points.get(A).distance(points.get(B));
        if (distAB > TRIPLE_DISTANCE_FACTOR * edgeThreshold) continue;
        for (int k = j+1; k < solIndices.size(); k++) {
          int C = solIndices.get(k);
          double distAC = points.get(A).distance(points.get(C));
          double distBC = points.get(B).distance(points.get(C));
          double maxDist = Math.max(distAB, Math.max(distAC, distBC));
          if (maxDist <= TRIPLE_DISTANCE_FACTOR * edgeThreshold) {
            candidateTriples.add(new int[]{A,B,C});
          }
        }
      }
    }

    candidateTriples.sort(Comparator.comparingDouble(triple -> {
      double distAB = points.get(triple[0]).distance(points.get(triple[1]));
      double distAC = points.get(triple[0]).distance(points.get(triple[2]));
      double distBC = points.get(triple[1]).distance(points.get(triple[2]));
      return Math.max(distAB, Math.max(distAC, distBC));
    }));

    if (candidateTriples.size() > TRIPLE_LIMIT) {
      candidateTriples = candidateTriples.subList(0, TRIPLE_LIMIT);
    }

    List<Integer> candidateAddPoints = selectTopCandidatePoints(dominated, inSol, CANDIDATE_LIMIT);

    ArrayList<Point> best = new ArrayList<>(solution);
    int bestSize = best.size();

    for (int[] triple : candidateTriples) {
      int A = triple[0], B = triple[1], C = triple[2];

      for (int U : candidateAddPoints) {
        ArrayList<Point> newSolOne = new ArrayList<>(solution);
        newSolOne.remove(points.get(A));
        newSolOne.remove(points.get(B));
        newSolOne.remove(points.get(C));
        newSolOne.add(points.get(U));
        if (newSolOne.size() < bestSize && isSolutionValid(newSolOne)) {
          return cleanUselessDominatingPoints(newSolOne);
        }
      }

      for (int U : candidateAddPoints) {
        for (int V : candidateAddPoints) {
          if (V == U) continue;
          ArrayList<Point> newSolTwo = new ArrayList<>(solution);
          newSolTwo.remove(points.get(A));
          newSolTwo.remove(points.get(B));
          newSolTwo.remove(points.get(C));
          newSolTwo.add(points.get(U));
          newSolTwo.add(points.get(V));
          if (newSolTwo.size() < bestSize && isSolutionValid(newSolTwo)) {
            return cleanUselessDominatingPoints(newSolTwo);
          }
        }
      }
    }

    return best;
  }

  /**
   * 从非解点中选出前K个高覆盖力点（对于当前未支配点），提高搜索速度与质量
   */
  private List<Integer> selectTopCandidatePoints(boolean[] dominated, Set<Integer> inSol, int limit) {
    int n = points.size();

    List<int[]> coverScores = new ArrayList<>();
    for (int i = 0; i < n; i++) {
      if (!inSol.contains(i)) {
        int score = countCoverage(i, dominated);
        coverScores.add(new int[]{i, score});
      }
    }
    if (coverScores.size() > limit) {
      coverScores = coverScores.subList(0, limit);
    }
    return coverScores.stream().map(a->a[0]).collect(Collectors.toList());
  }

  private boolean[] computeDominated(ArrayList<Point> solution) {
    boolean[] dominated = new boolean[points.size()];
    for (Point p : solution) {
      int idx = indexOf(p);
      markDominated(idx, dominated);
    }
    return dominated;
  }

  private boolean isFullyDominated(boolean[] dominated) {
    for (boolean b : dominated) {
      if (!b) return false;
    }
    return true;
  }

  private boolean isSolutionValid(ArrayList<Point> solution) {
    boolean[] dominated = computeDominated(solution);
    return isFullyDominated(dominated);
  }

  private ArrayList<Point> cleanUselessDominatingPoints(ArrayList<Point> dominating) {
    ArrayList<Point> res = new ArrayList<>(dominating);
    for (int i = 0; i < res.size(); i++) {
      Point p = res.get(i);
      ArrayList<Point> test = new ArrayList<>(res);
      test.remove(p);
      if (!isSolutionValid(test)) {
        // 保留p
      } else {
        res = test;
        i--;
      }
    }
    return res;
  }

  private ArrayList<Point> simulatedAnnealing(ArrayList<Point> currentSolution) {
    double temperature = 1000.0;
    double coolingRate = 0.999;
    int maxIterations = 50000;

    ArrayList<Point> bestSolution = new ArrayList<>(currentSolution);
    int bestCost = bestSolution.size();
    Random random = new Random();

    for (int iteration = 0; iteration < maxIterations; iteration++) {
      ArrayList<Point> newSolution = generateNeighbor(currentSolution, random);

      if (isSolutionValid(newSolution)) {
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

      temperature *= coolingRate;
      if (temperature < 1e-24) {
        break;
      }
    }

    return bestSolution;
  }

  private ArrayList<Point> generateNeighbor(ArrayList<Point> solution, Random random) {
    ArrayList<Point> newSolution = new ArrayList<>(solution);
    int operation = random.nextInt(3);

    Set<Point> solSet = new HashSet<>(solution);
    if (operation == 0 && !solution.isEmpty()) {
      int indexToRemove = random.nextInt(newSolution.size());
      newSolution.remove(indexToRemove);
    } else if (operation == 1) {
      ArrayList<Point> candidates = new ArrayList<>(points);
      candidates.removeAll(solSet);
      if (!candidates.isEmpty()) {
        int indexToAdd = random.nextInt(candidates.size());
        newSolution.add(candidates.get(indexToAdd));
      }
    } else if (operation == 2 && !solution.isEmpty()) {
      int indexToReplace = random.nextInt(newSolution.size());
      Point pointToRemove = newSolution.get(indexToReplace);
      ArrayList<Point> candidates = new ArrayList<>(points);
      candidates.removeAll(solSet);
      if (!candidates.isEmpty()) {
        int indexToAdd = random.nextInt(candidates.size());
        Point pointToAdd = candidates.get(indexToAdd);
        newSolution.set(indexToReplace, pointToAdd);
      } else {
        newSolution.remove(pointToRemove);
      }
    }

    return cleanUselessDominatingPoints(newSolution);
  }

}
