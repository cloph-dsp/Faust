// @category BLUR

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Namespace;

import java.io.BufferedWriter;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class ExportFunctionSummary extends GhidraScript {

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File outputDir = args.length > 0
            ? new File(args[0])
            : new File(System.getProperty("user.home"), "ghidra-exports");

        if (!outputDir.isDirectory() && !outputDir.mkdirs()) {
            throw new IllegalStateException("Failed to create output directory: " + outputDir);
        }

        List<Function> functions = new ArrayList<>();
        var iterator = currentProgram.getFunctionManager().getFunctions(true);
        while (iterator.hasNext()) {
            functions.add(iterator.next());
        }

        functions.sort(Comparator.comparing(function -> function.getEntryPoint().toString()));

        StringBuilder json = new StringBuilder();
        json.append("{\n");
        json.append("  \"program_name\": \"").append(jsonEscape(currentProgram.getName())).append("\",\n");
        json.append("  \"language_id\": \"").append(jsonEscape(String.valueOf(currentProgram.getLanguageID()))).append("\",\n");
        json.append("  \"image_base\": \"").append(jsonEscape(String.valueOf(currentProgram.getImageBase()))).append("\",\n");
        json.append("  \"function_count\": ").append(functions.size()).append(",\n");
        json.append("  \"functions\": [\n");

        for (int index = 0; index < functions.size(); index++) {
            Function function = functions.get(index);
            Namespace namespace = function.getParentNamespace();
            long size = function.getBody().getNumAddresses();

            json.append("    {\n");
            json.append("      \"name\": \"").append(jsonEscape(function.getName(true))).append("\",\n");
            json.append("      \"entry\": \"").append(jsonEscape(function.getEntryPoint().toString())).append("\",\n");
            json.append("      \"size\": ").append(size).append(",\n");
            json.append("      \"namespace\": \"")
                .append(jsonEscape(namespace != null ? namespace.getName() : ""))
                .append("\",\n");
            json.append("      \"external\": ").append(function.isExternal()).append(",\n");
            json.append("      \"thunk\": ").append(function.isThunk()).append("\n");
            json.append("    }");
            if (index + 1 < functions.size()) {
                json.append(',');
            }
            json.append("\n");
        }

        json.append("  ]\n");
        json.append("}\n");

        String fileName = sanitize(currentProgram.getName()) + "__" +
            sanitize(String.valueOf(currentProgram.getLanguageID())) + "_functions.json";
        File outputFile = new File(outputDir, fileName);

        try (BufferedWriter writer = Files.newBufferedWriter(outputFile.toPath(), StandardCharsets.UTF_8)) {
            writer.write(json.toString());
        }

        println("Wrote function summary to " + outputFile.getAbsolutePath());
    }

    private String sanitize(String value) {
        return value.replaceAll("[^A-Za-z0-9._-]", "_");
    }

    private String jsonEscape(String value) {
        return value
            .replace("\\", "\\\\")
            .replace("\"", "\\\"")
            .replace("\r", "\\r")
            .replace("\n", "\\n")
            .replace("\t", "\\t");
    }
}
